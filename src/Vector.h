// Created by Franz Seckel on 08.10.2025.
#pragma once
#include <algorithm>

template<typename T>
class Vector {
public:
    // Aliases:
    using valueType = T;
    using sizeType = size_t;
    using reference = valueType&;
    using constReference = const valueType&;
    using pointer = valueType*;
    using constPointer = const valueType*;
    using iterator = pointer;
    using constIterator = constPointer;
    using reverseIterator = std::reverse_iterator<iterator>;
    using constReverseIterator = std::reverse_iterator<constIterator>;

    // Constructors / Assignment / Destructor:
    constexpr Vector() noexcept = default;

    explicit Vector(sizeType size) :
        size_(size), capacity_(size), data_(allocate(size)) {
        std::uninitialized_value_construct_n(data_, size);
    }
    Vector(std::initializer_list<valueType> initList) :
        size_(initList.size()), capacity_(initList.size()), data_(allocate(initList.size())) {
        std::uninitialized_copy(initList.begin(), initList.end(), data_);
    }
    Vector(const Vector& other) :
        size_(other.size_), capacity_(other.capacity_), data_(allocate(other.capacity_)) {
        std::uninitialized_copy(other.data_, other.data_ + other.size_, data_); // NOLINT
    }
    Vector(Vector&& other) noexcept :
        size_(std::exchange(other.size_, 0)),
        capacity_(std::exchange(other.capacity_, 0)),
        data_(std::exchange(other.data_, nullptr)) {
    }
    ~Vector() noexcept { destroy(); }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            if (capacity_ < other.capacity_) {
                destroy();
                capacity_ = other.capacity_;
                data_ = allocate(other.capacity_);
            }
            size_ = other.size_;
            std::copy(other.data_, other.data_ + other.size_, data_);
        }
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            destroy();
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
            data_ = std::exchange(other.data_, nullptr);
        }
        return *this;
    }

    // Capacity:
    constexpr sizeType size() const noexcept { return size_; }
    constexpr sizeType capacity() const noexcept { return capacity_; }

    constexpr bool isEmpty() const noexcept { return size_ == 0; }
    constexpr bool isFull() const noexcept { return size_ == capacity_; }

    void reserve(const sizeType newCapacity) {
        if (capacity_ < newCapacity) reallocate(newCapacity);
    }
    void resize(const sizeType newSize) {
        if (newSize < size_)
            std::destroy(data_ + newSize, data_ + size_);
        else if (size_ < newSize) {
            reserve(newSize);
            std::uninitialized_value_construct_n(data_ + size_, newSize - size_);
        }
        size_ = newSize;
    }
    void shrinkToFit() { if (capacity_ > size_) reallocate(size_); }

    // Element Access:
    constexpr pointer data() noexcept { return data_; }
    constexpr constPointer data() const noexcept { return data_; }

    constexpr reference front() noexcept { return data_[0]; }
    constexpr constReference front() const noexcept { return data_[0]; }

    constexpr reference back() noexcept { return data_[size_ - 1]; }
    constexpr constReference back() const noexcept { return data_[size_ - 1]; }

    constexpr reference operator[](const sizeType index) noexcept { return data_[index]; }
    constexpr constReference operator[](const sizeType index) const noexcept { return data_[index]; }

    constexpr reference at(const sizeType index) {
        if (index >= size_) throw std::out_of_range("Vector::at");
        return data_[index];
    }
    constexpr constReference at(const sizeType index) const {
        if (index >= size_) throw std::out_of_range("Vector::at");
        return data_[index];
    }

    // Modifiers:
    template <typename Universal>
    void insertAt(const sizeType index, Universal&& value) {
        if (index > size_) throw std::out_of_range("Vector::insertAt");
        if (isFull()) reallocate(growCapacity());
        if (index < size_) std::move_backward(data_ + index, data_ + size_, data_ + size_ + 1);
        std::construct_at(data_ + index, std::forward<Universal>(value));
        ++size_;
    }
    template <typename Universal>
    void pushBack(Universal&& value) { insertAt(size_, std::forward<Universal>(value)); }

    template <typename... Arguments>
    void emplaceBack(Arguments&&... values) {
        if (isFull()) reallocate(growCapacity());
        std::construct_at(data_ + size_, std::forward<Arguments>(values)...);
        ++size_;
    }

    template <typename Universal>
    void assign(const sizeType count, Universal&& value) {
        clear();
        if (capacity_ < count) reallocate(count);
        std::uninitialized_fill_n(data_, count, std::forward<Universal>(value));
        size_ = count;
    }

    void removeAt(sizeType index) {
        if (index >= size_) throw std::out_of_range("Vector::removeAt");
        std::destroy_at(data_ + index);
        if (index < size_ - 1) std::move(data_ + index + 1, data_ + size_, data_ + index);
        --size_;
    }
    void popBack() { removeAt(size_ - 1); }

    void clear() {
        std::destroy(data_, data_ + size_);
        size_ = 0;
    }

    template <typename Universal>
    void update(sizeType index, Universal&& value) {
        if (index >= size_) throw std::out_of_range("Vector::update");;
        data_[index] = std::forward<Universal>(value);
    }

    void swap(Vector& other) noexcept(std::is_nothrow_swappable_v<valueType>) {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    // Comparison Operators:
    constexpr bool operator==(const Vector& other) const noexcept {
        if (size_ != other.size_) return false;
        for (sizeType i = 0; i < size_; ++i) {
            if (!(data_[i] == other.data_[i])) return false;
        }
        return true;
    }
    constexpr bool operator!=(const Vector& other) const noexcept { return !(*this == other); }
    constexpr bool operator<(const Vector& other) const noexcept {
        const sizeType minSize = std::min(size_, other.size_);
        for (sizeType i = 0; i < minSize; ++i) {
            if (data_[i] < other.data_[i]) return true;
            if (data_[i] > other.data_[i]) return false;
        }
        return size_ < other.size_;
    }
    constexpr bool operator>(const Vector& other) const noexcept { return other < *this; }
    constexpr bool operator<=(const Vector& other) const noexcept { return !(other < *this); }
    constexpr bool operator>=(const Vector& other) const noexcept { return !(*this < other); }

    // Iterators:
    iterator begin() noexcept { return data_; }
    constIterator begin() const noexcept { return data_; }
    constIterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return data_ + size_; }
    constIterator end() const noexcept { return data_ + size_; }
    constIterator cend() const noexcept { return data_ + size_; }

    reverseIterator rbegin() noexcept { return reverseIterator(end()); }
    constReverseIterator rbegin() const noexcept { return constReverseIterator(end()); }
    constReverseIterator crbegin() const noexcept { return constReverseIterator(end()); }

    reverseIterator rend() noexcept { return reverseIterator(begin()); }
    constReverseIterator rend() const noexcept { return constReverseIterator(begin()); }
    constReverseIterator crend() const noexcept { return constReverseIterator(begin()); }

    iterator find(const valueType& value) noexcept {
        for (sizeType i = 0; i < size_; ++i) {
            if (data_[i] == value) return data_ + i;
        }
        return data_ + size_;
    }
    constIterator find(const valueType& value) const noexcept {
        for (sizeType i = 0; i < size_; ++i) {
            if (data_[i] == value) return data_ + i;
        }
        return data_ + size_;
    }

private:
    sizeType size_ = 0;
    sizeType capacity_ = 0;
    pointer data_ = nullptr;

    static pointer allocate(sizeType size) {
        return size ? static_cast<pointer>(::operator new(size * sizeof(valueType))) : nullptr;
    }

    void destroy() noexcept {
        if (!data_) return;
        std::destroy(data_, data_ + size_);
        ::operator delete(data_);
    }

    void reallocate(const sizeType newCapacity) {
        pointer newData = allocate(newCapacity);
        try {
            std::uninitialized_move(data_, data_ + size_, newData);
        } catch (...) {
            ::operator delete(newData);
            throw;
        }
        destroy();
        data_ = newData;
        capacity_ = newCapacity;
    }

    sizeType growCapacity() const noexcept {
        return capacity_ == 0 ? 8 : capacity_ * 2;
    }
};

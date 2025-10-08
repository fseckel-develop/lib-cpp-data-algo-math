// Created by Franz Seckel on 07.10.2025.
#pragma once
#include <iterator>
#include <stdexcept>

template<typename T, size_t S>
class Array {
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
    constexpr Array() = default;
    constexpr Array(std::initializer_list<valueType> initList) {
        sizeType i = 0;
        for (const valueType& value : initList) {
            if (i >= S) break;
            data_[i++] = value;
        }
    }
    constexpr Array(const Array& other) = default;
    constexpr Array(Array&& other) noexcept = default;
    constexpr ~Array() = default;

    constexpr Array& operator=(const Array& other) = default;
    constexpr Array& operator=(Array&& other) noexcept = default;

    // Capacity:
    constexpr sizeType size() { return S; }     // NOLINT
    constexpr sizeType maxSize() { return S; }  // NOLINT
    constexpr bool isEmpty() { return S == 0; } // NOLINT

    // Element Access:
    constexpr pointer data() noexcept { return data_; }
    constexpr constPointer data() const noexcept { return data_; }

    constexpr reference front() noexcept { return data_[0]; }
    constexpr constReference front() const noexcept { return data_[0]; }

    constexpr reference back() noexcept { return data_[S - 1]; }
    constexpr constReference back() const noexcept { return data_[S - 1]; }

    constexpr reference operator[](sizeType index) noexcept { return data_[index]; }
    constexpr constReference operator[](sizeType index) const noexcept { return data_[index]; }

    constexpr reference at(sizeType index) {
        if (index >= S) throw std::out_of_range("Array::at");
        return data_[index];
    }
    constexpr constReference at(sizeType index) const {
        if (index >= S) throw std::out_of_range("Array::at");
        return data_[index];
    }

    // Modifiers:
    constexpr void fill(const valueType& value) noexcept(noexcept(std::declval<valueType&>() = value)) {
        for (valueType& element : data_)
            element = value;
    }
    constexpr void swap(Array& other) noexcept(std::is_nothrow_swappable_v<valueType>) {
        for (sizeType i = 0; i < S; ++i)
            std::swap(data_[i], other.data_[i]);
    }

    // Comparison Operators:
    constexpr bool operator==(const Array& other) const noexcept = default;
    constexpr bool operator!=(const Array& other) const noexcept { return !(*this == other); }
    constexpr bool operator<(const Array& other) const noexcept {
        for (sizeType i = 0; i < S; ++i) {
            if (data_[i] < other.data_[i]) return true;
            if (data_[i] > other.data_[i]) return false;
        }
        return false;
    }
    constexpr bool operator>(const Array& other) const noexcept { return other < *this; }
    constexpr bool operator<=(const Array& other) const noexcept { return !(other < *this); }
    constexpr bool operator>=(const Array& other) const noexcept { return !(*this < other); }

    // Iterators:
    constexpr iterator begin() noexcept { return iterator(data()); }
    constexpr constIterator begin() const noexcept { return constIterator(data()); }
    constexpr constIterator cbegin() const noexcept { return begin(); }

    constexpr iterator end() noexcept { return iterator(data() + S); }
    constexpr constIterator end() const noexcept { return constIterator(data() + S); }
    constexpr constIterator cend() const noexcept { return end(); }

    constexpr reverseIterator rbegin() noexcept { return reverseIterator(end()); }
    constexpr constReverseIterator rbegin() const noexcept { return constReverseIterator(end()); }
    constexpr constReverseIterator crbegin() const noexcept { return constReverseIterator(end()); }

    constexpr reverseIterator rend() noexcept { return reverseIterator(begin()); }
    constexpr constReverseIterator rend() const noexcept { return constReverseIterator(begin()); }
    constexpr constReverseIterator crend() const noexcept { return constReverseIterator(begin()); }

private:
    T data_[S] = {};
};

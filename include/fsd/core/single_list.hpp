// Created by Franz Seckel on 11.10.2025.
#pragma once
#include <memory>
#include <cassert>

namespace fsd::core
{
    template <typename T>
    class SingleList {
        struct Node {
            T data;
            Node* next = nullptr;
            explicit Node(const T& data) : data(data) {}
        };

    public:
        // Aliases:
        using valueType = T;
        using sizeType = size_t;
        using reference = valueType&;
        using constReference = const valueType&;
        using pointer = valueType*;
        using constPointer = const valueType*;

        // Constructors / Assignment / Destructor:
        SingleList() noexcept = default;
        SingleList(std::initializer_list<T> initList) {
            for (const auto& value : initList) pushBack(value);
        }
        SingleList(const SingleList& other) { copyFrom(other); }
        SingleList(SingleList&& other) noexcept { moveFrom(std::move(other)); }

        SingleList& operator=(const SingleList& other) {
            if (this != &other) copyFrom(other);
            return *this;
        }
        SingleList& operator=(SingleList&& other) noexcept {
            if (this != &other) { clear(); moveFrom(std::move(other)); }
            return *this;
        }

        ~SingleList() noexcept { clear(); }

        // Capacity:
        [[nodiscard]] sizeType size() const noexcept { return size_; }
        [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

        // Element Access:
        reference front() noexcept { return head_->data; }
        constReference front() const noexcept { return head_->data; }

        reference back() noexcept { return tail_->data; }
        constReference back() const noexcept { return tail_->data; }

        reference operator[](const sizeType index) {
            assert(index < size_ && "SingleList::operator[] index out of range");
            return getNodeAt(index)->data;
        }
        constReference operator[](const sizeType index) const {
            assert(index < size_ && "SingleList::operator[] index out of range");
            return getNodeAt(index)->data;
        }

        reference at(const sizeType index) {
            if (index >= size_) throw std::out_of_range("SingleList::at");
            return (*this)[index];
        }
        constReference at(const sizeType index) const {
            if (index >= size_) throw std::out_of_range("SingleList::at");
            return (*this)[index];
        }

        // Modifiers:
        template <typename Universal>
        void insertAt(const sizeType index, Universal&& value) {
            if (index > size_) throw std::out_of_range("SingleList::insertAt");
            Node* newNode = new Node(value);
            if (index == 0) {
                newNode->next = head_;
                head_ = newNode;
                if (!tail_) tail_ = newNode;
            }
            else if (index == size_) {
                tail_->next = newNode;
                tail_ = newNode;
            }
            else {
                Node* prev = getNodeAt(index - 1);
                newNode->next = prev->next;
                prev->next = newNode;
            }
            ++size_;
        }
        template <typename Universal>
        void pushFront(Universal&& value) { insertAt(0, std::forward<Universal>(value)); }
        template <typename Universal>
        void pushBack(Universal&& value) { insertAt(size_, std::forward<Universal>(value)); }

        void removeAt(const sizeType index) {
            if (index >= size_) throw std::out_of_range("SingleList::removeAt");
            if (index == 0) {
                const Node* target = head_;
                head_ = head_->next;
                if (size_ == 1) tail_ = nullptr;
                delete target;
            }
            else {
                Node* prev = getNodeAt(index - 1);
                const Node* target = prev->next;
                prev->next = target->next;
                if (target == tail_) tail_ = prev;
                delete target;
            }
            --size_;
        }
        void popFront() noexcept { removeAt(0); }
        void popBack() noexcept { removeAt(size_ - 1); }

        template <typename Universal>
        void update(const sizeType index, Universal&& value) {
            if (index >= size_) throw std::out_of_range("SingleList::update");
            getNodeAt(index)->data = std::forward<Universal>(value);
        }
        void clear() noexcept {
            Node* currentNode = head_;
            while (currentNode) {
                const Node* target = currentNode;
                currentNode = currentNode->next;
                delete target;
            }
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
        }

        // Comparison Operators:
        bool operator==(const SingleList& other) const noexcept {
            if (size_ != other.size_) return false;
            const Node* currentNode = head_;
            const Node* otherNode = other.head_;
            while (currentNode && otherNode) {
                if (currentNode->data != otherNode->data) return false;
                currentNode = currentNode->next;
                otherNode = otherNode->next;
            }
            return true;
        }
        bool operator<(const SingleList& other) const noexcept {
            const Node* currentNode = head_;
            const Node* otherNode = other.head_;
            while (currentNode && otherNode) {
                if (currentNode->data < otherNode->data) return true;
                if (currentNode->data > otherNode->data) return false;
                currentNode = currentNode->next;
                otherNode = otherNode->next;
            }
            return currentNode == nullptr && otherNode != nullptr;
        }
        bool operator!=(const SingleList& other) const noexcept { return !(*this == other); }
        bool operator>(const SingleList& other) const noexcept { return other < *this; }
        bool operator<=(const SingleList& other) const noexcept { return !(other < *this); }
        bool operator>=(const SingleList& other) const noexcept { return !(*this < other); }

        // Iterators:
        class iterator {
            Node* current_;
        public:
            explicit iterator(Node* node) : current_(node) {}
            reference operator*() const noexcept { return current_->data; }
            pointer operator->() const noexcept { return &current_->data; }
            iterator& operator++() noexcept { current_ = current_->next; return *this; }
            iterator operator++(int) noexcept { iterator temp = *this; current_ = current_->next; return temp; }
            bool operator==(const iterator& other) const noexcept { return current_ == other.current_; }
            bool operator!=(const iterator& other) const noexcept { return current_ != other.current_; }
        };

        class constIterator {
            const Node* current_;
        public:
            explicit constIterator(const Node* node) : current_(node) {}
            constReference operator*() const noexcept { return current_->data; }
            constPointer operator->() const noexcept { return &current_->data; }
            constIterator& operator++() noexcept { current_ = current_->next; return *this; }
            constIterator operator++(int) noexcept { constIterator temp = *this; current_ = current_->next; return temp; }
            bool operator==(const constIterator& other) const noexcept { return current_ == other.current_; }
            bool operator!=(const constIterator& other) const noexcept { return current_ != other.current_; }
        };

        iterator begin() noexcept { return iterator(head_); }
        constIterator begin() const noexcept { return constIterator(head_); }
        constIterator cbegin() const noexcept { return constIterator(head_); }

        iterator end() noexcept { return iterator(nullptr); }
        constIterator end() const noexcept { return constIterator(nullptr); }
        constIterator cend() const noexcept { return constIterator(nullptr); }

        iterator find(const valueType& value) noexcept {
            Node* currentNode = head_;
            while (currentNode) {
                if (currentNode->data == value) return iterator(currentNode);
                currentNode = currentNode->next;
            }
            return iterator(nullptr);
        }
        constIterator find(const valueType& value) const noexcept {
            const Node* currentNode = head_;
            while (currentNode) {
                if (currentNode->data == value) return constIterator(currentNode);
                currentNode = currentNode->next;
            }
            return constIterator(nullptr);
        }

    private:
        Node* head_ = nullptr;
        Node* tail_ = nullptr;
        sizeType size_ = 0;

        void copyFrom(const SingleList& other) {
            clear();
            for (const auto& value : other) { pushBack(value); }
        }

        void moveFrom(SingleList&& other) noexcept {
            head_ = std::exchange(other.head_, nullptr);
            tail_ = std::exchange(other.tail_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }

        Node* getNodeAt(const sizeType index) const noexcept {
            if (index >= size_) return nullptr;
            Node* currentNode = head_;
            for (sizeType i = 0; i < index; ++i) currentNode = currentNode->next;
            return currentNode;
        }
    };
} // namespace fsd::core
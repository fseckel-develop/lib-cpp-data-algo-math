// Created by Franz Seckel on 12.10.2025.
#pragma once
#include <cassert>
#include <algorithm>

namespace fsd::core
{
    template <typename T>
    class Deque {
    public:
        // Aliases:
        using valueType = T;
        using sizeType = size_t;
        using reference = valueType&;
        using constReference = const valueType&;
        using pointer = valueType*;
        using constPointer = const valueType*;

        // Constructors / Assignment / Destructor:
        Deque() {
            blocks_ = new pointer[blockCount_];
            for (sizeType i = 0; i < blockCount_; i++) blocks_[i] = nullptr;
            blocks_[startBlock_] = new valueType[BLOCK_SIZE];
        }
        Deque(std::initializer_list<T> initList) : Deque() {
            for (const auto& value : initList) pushBack(value);
        }
        Deque(const Deque& other) :
            blockCount_(other.blockCount_), startBlock_(other.startBlock_), startOffset_(other.startOffset_),
            endBlock_(other.endBlock_), endOffset_(other.endOffset_), size_(other.size_) {
            blocks_ = new pointer[blockCount_];
            for (sizeType i = 0; i < blockCount_; i++) {
                if (other.blocks_[i]) {
                    blocks_[i] = new valueType[BLOCK_SIZE];
                    std::copy(other.blocks_[i], other.blocks_[i] + BLOCK_SIZE, blocks_[i]);
                }
                else blocks_[i] = nullptr;
            }
        }
        Deque(Deque&& other) noexcept :
            blocks_(other.blocks_), blockCount_(other.blockCount_), startBlock_(other.startBlock_),
            startOffset_(other.startOffset_), endBlock_(other.endBlock_), endOffset_(other.endOffset_), size_(other.size_) {
            other.blocks_ = nullptr;
            other.size_ = 0;
        }

        ~Deque() {
            for (sizeType i = 0; i < blockCount_; i++) delete[] blocks_[i];
            delete[] blocks_;
        }

        Deque& operator=(const Deque& other) {
            if (this == &other) return *this;
            Deque temp(other);
            swap(temp);
            return *this;
        }
        Deque& operator=(Deque&& other) noexcept {
            if (this == &other) return *this;
            clear();
            delete[] blocks_;
            blocks_ = other.blocks_;
            startBlock_ = other.startBlock_;
            startOffset_ = other.startOffset_;
            endBlock_ = other.endBlock_;
            endOffset_ = other.endOffset_;
            size_ = other.size_;
            other.blocks_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        // Capacity:
        [[nodiscard]] sizeType size() const noexcept { return size_; }
        [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

        // Element Access:
        reference front() noexcept { return blocks_[startBlock_][startOffset_]; }
        constReference front() const noexcept { return blocks_[startBlock_][startOffset_]; }

        reference back() noexcept { return blocks_[endBlock_][endOffset_ - 1]; }
        constReference back() const noexcept { return blocks_[endBlock_][endOffset_ - 1]; }

        reference operator[](const sizeType index) {
            assert(index < size_ && "Deque::operator[] index out of range");
            auto [blockIndex, offset] = locate(index);
            return blocks_[blockIndex][offset];
        }
        constReference operator[](const sizeType index) const {
            assert(index < size_ && "Deque::operator[] index out of range");
            auto [blockIndex, offset] = locate(index);
            return blocks_[blockIndex][offset];
        }

        reference at(const sizeType index) {
            if (index >= size_) throw std::out_of_range("Deque::at");
            return (*this)[index];
        }
        constReference at(const sizeType index) const {
            if (index >= size_) throw std::out_of_range("Deque::at");
            return (*this)[index];
        }

        // Modifiers:
        template <typename Universal>
        void pushFront(Universal&& value) {
            if (startOffset_ == 0) {
                if (startBlock_ == 0)
                    if (!rebalance()) reallocate();
                --startBlock_;
                allocateBlockIfNeeded(startBlock_);
                startOffset_ = BLOCK_SIZE;
            }
            blocks_[startBlock_][--startOffset_] = std::forward<Universal>(value);
            ++size_;
        }
        template <typename Universal>
        void pushBack(Universal&& value) {
            if (endOffset_ == BLOCK_SIZE) {
                if (endBlock_ + 1 == blockCount_)
                    if (!rebalance()) reallocate();
                ++endBlock_;
                allocateBlockIfNeeded(endBlock_);
                endOffset_ = 0;
            }
            blocks_[endBlock_][endOffset_++] = std::forward<Universal>(value);
            ++size_;
        }

        void popFront() {
            if (size_ == 0) return;
            ++startOffset_;
            --size_;
            if (startOffset_ == BLOCK_SIZE && size_ > 0) {
                delete[] blocks_[startBlock_];
                blocks_[startBlock_++] = nullptr;
                startOffset_ = 0;
            }
        }
        void popBack() {
            if (size_ == 0) return;
            --endOffset_;
            --size_;
            if (endOffset_ == 0 && size_ > 0) {
                delete[] blocks_[endBlock_];
                blocks_[endBlock_--] = nullptr;
                endOffset_ = BLOCK_SIZE;
            }
        }

        void clear() {
            startBlock_ = endBlock_ = blockCount_ / 2;
            startOffset_ = endOffset_ = 0;
            size_ = 0;
        }

        void swap(Deque& other) noexcept {
            std::swap(blocks_, other.blocks_);
            std::swap(blockCount_, other.blockCount_);
            std::swap(startBlock_, other.startBlock_);
            std::swap(startOffset_, other.startOffset_);
            std::swap(endBlock_, other.endBlock_);
            std::swap(endOffset_, other.endOffset_);
            std::swap(size_, other.size_);
        }

        // Iterators:
        class iterator {
            pointer* blocks_ = nullptr;
            sizeType blockIndex_ = 0;
            sizeType offset_ = 0;
        public:
            iterator(const pointer* blocks, const sizeType blockIndex, const sizeType offset) :
                blocks_(blocks), blockIndex_(blockIndex), offset_(offset) {
            }
            reference operator*() const noexcept { return blocks_[blockIndex_][offset_]; }
            pointer operator->() const noexcept { return &blocks_[blockIndex_][offset_]; }
            iterator& operator++() noexcept {
                ++offset_;
                if (offset_ == BLOCK_SIZE) { offset_ = 0; ++blockIndex_; }
                return *this;
            }
            iterator operator++(int) noexcept { iterator temp = *this; ++(*this); return temp; }
            iterator& operator--() noexcept {
                if (offset_ == 0) { offset_ = BLOCK_SIZE; --blockIndex_; }
                --offset_;
                return *this;
            }
            iterator operator--(int) noexcept { iterator temp = *this; --(*this); return temp; }
            bool operator==(const iterator& other) const noexcept {
                return blockIndex_ == other.blockIndex_ && offset_ == other.offset_;
            }
            bool operator!=(const iterator& other) const noexcept { return !(*this == other); }
        };

        class constIterator {
            const pointer* blocks_ = nullptr;
            sizeType blockIndex_ = 0;
            sizeType offset_ = 0;
        public:
            constIterator(const pointer* blocks, const sizeType blockIndex, const sizeType offset) noexcept
                : blocks_(blocks), blockIndex_(blockIndex), offset_(offset) {
            }
            constReference operator*() const noexcept { return blocks_[blockIndex_][offset_]; }
            constPointer operator->() const noexcept { return &blocks_[blockIndex_][offset_]; }
            constIterator& operator++() noexcept {
                ++offset_;
                if (offset_ == BLOCK_SIZE) { offset_ = 0; ++blockIndex_; }
                return *this;
            }
            constIterator operator++(int) noexcept { constIterator temp = *this; ++(*this); return temp; }
            constIterator& operator--() noexcept {
                if (offset_ == 0) { offset_ = BLOCK_SIZE; --blockIndex_; }
                --offset_;
                return *this;
            }
            constIterator operator--(int) noexcept { constIterator temp = *this; --(*this); return temp; }
            bool operator==(const constIterator& other) const noexcept {
                return blockIndex_ == other.blockIndex_ && offset_ == other.offset_;
            }
            bool operator!=(const constIterator& other) const noexcept { return !(*this == other); }
        };

        using reverseIterator = std::reverse_iterator<iterator>;
        using constReverseIterator = std::reverse_iterator<constIterator>;

        iterator begin() noexcept { return iterator(blocks_, startBlock_, startOffset_); }
        constIterator begin() const noexcept { return constIterator(blocks_, startBlock_, startOffset_); }
        constIterator cbegin() const noexcept { return begin(); }

        iterator end() noexcept { return iterator(blocks_, endBlock_, endOffset_); }
        constIterator end() const noexcept { return constIterator(blocks_, endBlock_, endOffset_); }
        constIterator cend() const noexcept { return end(); }

        reverseIterator rbegin() noexcept { return reverseIterator(end()); }
        constReverseIterator rbegin() const noexcept { return constReverseIterator(end()); }
        constReverseIterator crbegin() const noexcept { return constReverseIterator(cend()); }

        reverseIterator rend() noexcept { return reverseIterator(begin()); }
        constReverseIterator rend() const noexcept { return constReverseIterator(begin()); }
        constReverseIterator crend() const noexcept { return constReverseIterator(cbegin()); }

        // Comparison Operators:
        bool operator==(const Deque& other) const noexcept {
            if (size_ != other.size_) return false;
            return std::equal(cbegin(), cend(), other.cbegin());
        }
        bool operator!=(const Deque& other) const noexcept { return !(*this == other); }

        bool operator<(const Deque& other) const noexcept {
            return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
        }
        bool operator>(const Deque& other) const noexcept { return other < *this; }
        bool operator<=(const Deque& other) const noexcept { return !(other < *this); }
        bool operator>=(const Deque& other) const noexcept { return !(*this < other); }

    private:
        static constexpr size_t BLOCK_SIZE = 64;
        pointer* blocks_ = nullptr;
        sizeType blockCount_ = 8;
        sizeType startBlock_ = 4;
        sizeType startOffset_ = 0;
        sizeType endBlock_ = 4;
        sizeType endOffset_ = 0;
        sizeType size_ = 0;

        [[nodiscard]] std::pair<sizeType, sizeType> locate(const sizeType index) const {
            const sizeType absoluteIndex = startOffset_ + index;
            const sizeType blockIndex = startBlock_ + absoluteIndex / BLOCK_SIZE;
            const sizeType offset = absoluteIndex % BLOCK_SIZE;
            return {blockIndex, offset};
        }

        void allocateBlockIfNeeded(const sizeType blockIndex) {
            if (!blocks_[blockIndex]) blocks_[blockIndex] = new valueType[BLOCK_SIZE];
        }

        bool rebalance() {
            const sizeType activeBlocks = endBlock_ - startBlock_ + 1;
            const sizeType spaceLeft = startBlock_;
            const sizeType spaceRight = blockCount_ - (endBlock_ + 1); // NOLINT
            if (spaceLeft == 0 && spaceRight == 0) return false;
            if (spaceLeft < spaceRight) {
                const sizeType rightShift = (spaceRight + 1) / 2;
                for (sizeType i = activeBlocks; i > 0; i--)
                    blocks_[startBlock_ + rightShift + i] = blocks_[startBlock_ + i];
                for (sizeType i = 0; i < rightShift; i++)
                    blocks_[startBlock_ + i] = nullptr;
                startBlock_ += rightShift;
                endBlock_ += rightShift;
            } else {
                const sizeType leftShift = (spaceLeft + 1) / 2;
                for (sizeType i = 0; i < activeBlocks; i++)
                    blocks_[startBlock_ - leftShift + i] = blocks_[startBlock_ + i];
                for (sizeType i = 0; i < leftShift; i++)
                    blocks_[endBlock_ + i] = nullptr;
                startBlock_ -= leftShift;
                endBlock_ -= leftShift;
            }
            return true;
        }

        void reallocate() {
            blockCount_ = blockCount_ * 2;
            pointer* newBlocks = new pointer[blockCount_]; // NOLINT
            for (sizeType i = 0; i < blockCount_; i++)
                newBlocks[i] = nullptr;
            const sizeType activeBlocks = endBlock_ - startBlock_ + 1;
            const sizeType newStartBlock = (blockCount_ - activeBlocks) / 2;
            for (sizeType i = 0; i < activeBlocks; i++)
                newBlocks[newStartBlock + i] = blocks_[startBlock_ + i];
            delete[] blocks_;
            blocks_ = newBlocks;
            startBlock_ = newStartBlock;
            endBlock_ = newStartBlock + activeBlocks - 1;
        }
    };
} // namespace fsd::core

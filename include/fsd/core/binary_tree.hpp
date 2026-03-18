// Created by Franz Seckel on 04.11.2025.
#pragma once
#include <iostream>
#include <fsd/core/queue.hpp>
#include <fsd/core/stack.hpp>

namespace fsd::core
{
    template <typename T>
    class BinaryTree {
    protected:
        struct Node {
            T data;
            Node* left = nullptr;
            Node* right = nullptr;
            explicit Node(const T& data) : data(data) {}
        };

    public:
        // Aliases:
        using valueType = T;
        using sizeType = size_t;

        // Constructors / Assignment / Destructor:
        BinaryTree() : root_(nullptr) {}
        BinaryTree(const BinaryTree& other) : BinaryTree() {
            if (other.root_) root_ = copySubtree(other.root_);
            size_ = other.size_;
        }
        BinaryTree(BinaryTree&& other) noexcept :
            root_(std::exchange(other.root_, nullptr)),
            size_(std::exchange(other.size_, 0)) {
        }
        BinaryTree& operator=(const BinaryTree& other) {
            if (this == &other) return *this;
            clearSubtree(root_);
            root_ = nullptr;
            if (other.root_) root_ = copySubtree(other.root_);
            size_ = other.size_;
            return *this;
        }
        BinaryTree& operator=(BinaryTree&& other) noexcept {
            if (this == &other) return *this;
            clearSubtree(root_);
            root_ = std::exchange(other.root_, nullptr);
            size_ = std::exchange(other.size_, 0);
            return *this;
        }
        virtual ~BinaryTree() { clear(); }

        // Capacity + Height:
        [[nodiscard]] sizeType size() const noexcept { return size_; }
        [[nodiscard]] bool empty() const noexcept { return root_ == nullptr; }
        [[nodiscard]] int height() const noexcept { return heightSubtree(root_); }

        // Modifiers:
        virtual void insert(const valueType& value) {
            Node* newNode = new Node(value);
            ++size_;
            if (!root_) { root_ = newNode; return; }
            Queue<Node*> queue;
            queue.enqueue(root_);
            while (!queue.empty()) {
                Node* currentNode = queue.front();
                queue.dequeue();
                if (!currentNode->left) { currentNode->left = newNode; return; }
                queue.enqueue(currentNode->left);
                if (!currentNode->right) { currentNode->right = newNode; return; }
                queue.enqueue(currentNode->right);
            }
        }

        virtual void remove(const valueType& value) {
            if (!root_) return;
            Queue<Node*> queue;
            queue.enqueue(root_);
            Node* target = nullptr;
            Node* lastNode = nullptr;
            Node* parentOfLast = nullptr;
            while (!queue.empty()) {
                Node* currentNode = queue.front();
                queue.dequeue();
                if (currentNode->data == value)
                    target = currentNode;
                if (currentNode->left) {
                    parentOfLast = currentNode;
                    queue.enqueue(currentNode->left);
                }
                if (currentNode->right) {
                    parentOfLast = currentNode;
                    queue.enqueue(currentNode->right);
                }
                lastNode = currentNode;
            }
            if (target) {
                target->data = lastNode->data;
                if (parentOfLast) {
                    if (parentOfLast->right == lastNode) parentOfLast->right = nullptr;
                    else if (parentOfLast->left == lastNode) parentOfLast->left = nullptr;
                }
                else if (lastNode == root_) root_ = nullptr;
                delete lastNode;
                --size_;
            }
        }

        void clear() noexcept {
            clearSubtree(root_);
            root_ = nullptr;
            size_ = 0;
        }

        // Element Search:
        virtual bool search(const valueType& value) const { return searchInSubtree(root_, value); }

        // Traversal:
        Vector<valueType> preorder() const {
            Vector<valueType> result;
            preorderSubtree(root_, result);
            return result;
        }
        Vector<valueType> inorder() const {
            Vector<valueType> result;
            inorderSubtree(root_, result);
            return result;
        }
        Vector<valueType> postorder() const {
            Vector<valueType> result;
            postorderSubtree(root_, result);
            return result;
        }
        Vector<valueType> levelOrder() const {
            Vector<valueType> result;
            if (!root_) return result;
            Queue<Node*> queue;
            queue.enqueue(root_);
            while (!queue.empty()) {
                Node* currentNode = queue.front();
                queue.dequeue();
                result.pushBack(currentNode->data);
                if (currentNode->left) queue.enqueue(currentNode->left);
                if (currentNode->right) queue.enqueue(currentNode->right);
            }
            return result;
        }

        // Iterators:
        class iterator {
        public:
            using valueType = T;
            using reference = valueType&;
            using pointer = valueType*;

            explicit iterator(Node* root = nullptr) { pushLeft(root); }
            reference operator*() const noexcept { return nodeStack_.top()->data; }
            pointer operator->() const noexcept { return &(nodeStack_.top()->data); }
            iterator& operator++() noexcept {
                Node* currentNode = nodeStack_.top();
                nodeStack_.pop();
                if (currentNode->right) pushLeft(currentNode->right);
                return *this;
            }
            iterator operator++(int) noexcept {
                iterator temp = *this;
                ++(*this);
                return temp;
            }
            bool operator==(const iterator& other) const noexcept {
                if (nodeStack_.empty() && other.nodeStack_.empty()) return true;
                if (nodeStack_.empty() || other.nodeStack_.empty()) return false;
                return nodeStack_.top() == other.nodeStack_.top();
            }
            bool operator!=(const iterator& other) const noexcept { return !(*this == other); }

        private:
            Stack<Node*> nodeStack_;

            void pushLeft(Node* node) noexcept {
                while (node) {
                    nodeStack_.push(node);
                    node = node->left;
                }
            }
        };

        class constIterator {
        public:
            using valueType = T;
            using pointer = const valueType*;
            using reference = const valueType&;

            explicit constIterator(Node* root = nullptr) { pushLeft(root); }
            reference operator*() const noexcept { return nodeStack_.top()->data; }
            pointer operator->() const noexcept { return &(nodeStack_.top()->data); }
            constIterator& operator++() noexcept {
                Node* current = nodeStack_.top();
                nodeStack_.pop();
                if (current->right) pushLeft(current->right);
                return *this;
            }
            constIterator operator++(int) noexcept {
                constIterator temp = *this;
                ++(*this);
                return temp;
            }
            bool operator==(const constIterator& other) const noexcept {
                if (nodeStack_.empty() && other.nodeStack_.empty()) return true;
                if (nodeStack_.empty() || other.nodeStack_.empty()) return false;
                return nodeStack_.top() == other.nodeStack_.top();
            }
            bool operator!=(const constIterator& other) const noexcept { return !(*this == other); }

        private:
            Stack<Node*> nodeStack_;

            void pushLeft(Node* node) noexcept {
                while (node) {
                    nodeStack_.push(node);
                    node = node->left;
                }
            }
        };

        iterator begin() noexcept { return iterator(root_); }
        constIterator begin() const noexcept { return constIterator(root_); }
        constIterator cbegin() const noexcept { return constIterator(root_); }

        iterator end() noexcept { return iterator(nullptr); }
        constIterator end() const noexcept { return constIterator(nullptr); }
        constIterator cend() const noexcept { return constIterator(nullptr); }

    protected:
        Node* root_ = nullptr;
        sizeType size_ = 0;

        // Recursive Helper Methods:
        static Node* copySubtree(const Node* otherRoot) {
            if (!otherRoot) return nullptr;
            Node* newRoot = new Node(otherRoot->data);
            newRoot->left = copySubtree(otherRoot->left);
            newRoot->right = copySubtree(otherRoot->right);
            return newRoot;
        }

        static void clearSubtree(Node* root) {
            if (!root) return;
            clearSubtree(root->left);
            clearSubtree(root->right);
            delete root;
        }

        static bool searchInSubtree(Node* root, const valueType& value) noexcept {
            if (!root) return false;
            if (root->data == value) return true;
            return searchInSubtree(root->left, value) || searchInSubtree(root->right, value);
        }

        static int heightSubtree(Node* root) noexcept {
            if (!root) return -1;
            return 1 + std::max(heightSubtree(root->left), heightSubtree(root->right));
        }

        static void preorderSubtree(Node* root, Vector<valueType>& output) noexcept {
            if (root) {
                output.pushBack(root->data);
                preorderSubtree(root->left, output);
                preorderSubtree(root->right, output);
            }
        }
        static void inorderSubtree(Node* root, Vector<valueType>& output) noexcept {
            if (root) {
                inorderSubtree(root->left, output);
                output.pushBack(root->data);
                inorderSubtree(root->right, output);
            }
        }
        static void postorderSubtree(Node* root, Vector<valueType>& output) noexcept {
            if (root) {
                postorderSubtree(root->left, output);
                postorderSubtree(root->right, output);
                output.pushBack(root->data);
            }
        }
    };
} // namespace fsd::core

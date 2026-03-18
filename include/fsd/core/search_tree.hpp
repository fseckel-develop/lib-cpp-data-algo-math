// Created by Franz Seckel on 06.11.2025.
#pragma once
#include <fsd/core/binary_tree.hpp>

namespace fsd::core
{
    template <typename T>
    class SearchTree final : public BinaryTree<T> {
    public:
        // Aliases:
        using Node = typename BinaryTree<T>::Node;
        using valueType = typename BinaryTree<T>::valueType;
        using sizeType = typename BinaryTree<T>::sizeType;
        using iterator = typename BinaryTree<T>::iterator;
        using constIterator = typename BinaryTree<T>::constIterator;
        using BinaryTree<T>::root_;
        using BinaryTree<T>::size_;

        // Constructors / Assignment / Destructor:
        SearchTree() = default;
        SearchTree(const SearchTree&) = default;
        SearchTree(SearchTree&&) noexcept = default;
        SearchTree& operator=(const SearchTree&) = default;
        SearchTree& operator=(SearchTree&&) noexcept = default;
        ~SearchTree() override = default;

        // Modifiers:
        void insert(const valueType& value) override { root_ = insertInSubtree(root_, value); }
        void remove(const valueType& value) override { root_ = removeFromSubtree(root_, value); }

        // Element Search / Lookup:
        bool search(const valueType& value) const override {
            return searchInSubtree(root_, value);
        }
        iterator find(const valueType& value) noexcept {
            Node* currentNode = root_;
            while (currentNode) {
                if (value == currentNode->data) return iterator(currentNode);
                if (value < currentNode->data) currentNode = currentNode->left;
                else currentNode = currentNode->right;
            }
            return this->end();
        }
        constIterator find(const valueType& value) const noexcept {
            Node* currentNode = root_;
            while (currentNode) {
                if (value == currentNode->data) return constIterator(currentNode);
                if (value < currentNode->data) currentNode = currentNode->left;
                else currentNode = currentNode->right;
            }
            return this->end();
        }

        const valueType& min() const {
            Node* node = root_;
            if (!node) throw std::invalid_argument("Tree is empty");
            while (node->left) node = node->left;
            return node->data;
        }
        const valueType& max() const {
            Node* node = root_;
            if (!node) throw std::invalid_argument("Tree is empty");
            while (node->right) node = node->right;
            return node->data;
        }

        iterator lowerBound(const valueType& value) noexcept {
            Node* current = root_;
            Node* candidate = nullptr;
            while (current) {
                if (current->data < value) current = current->right;
                else {
                    candidate = current;
                    current = current->left;
                }
            }
            return candidate ? iterator(candidate) : this->end();
        }
        iterator upperBound(const valueType& value) noexcept {
            Node* current = root_;
            Node* candidate = nullptr;
            while (current) {
                if (value < current->data) {
                    candidate = current;
                    current = current->left;
                }
                else current = current->right;
            }
            return candidate ? iterator(candidate) : this->end();
        }

    private:
        // Recursive Helper Methods:
        Node* insertInSubtree(Node* root, const valueType& value) {
            if (!root) {
                ++size_;
                return new Node(value);
            }
            if (value < root->data)
                root->left = insertInSubtree(root->left, value);
            else if (value > root->data)
                root->right = insertInSubtree(root->right, value);
            return root;
        }

        Node* removeFromSubtree(Node* root, const valueType& value) {
            if (!root) return nullptr;
            if (value < root->data)
                root->left = removeFromSubtree(root->left, value);
            else if (value > root->data)
                root->right = removeFromSubtree(root->right, value);
            else {
                if (!root->left && !root->right) {
                    delete root;
                    --size_;
                    return nullptr;
                }
                if (!root->left) {
                    Node* remainder = root->right;
                    delete root;
                    --size_;
                    return remainder;
                }
                if (!root->right) {
                    Node* remainder = root->left;
                    delete root;
                    --size_;
                    return remainder;
                }
                Node* successor = root->right;
                while (successor->left) successor = successor->left;
                root->data = successor->data;
                root->right = removeFromSubtree(root->right, successor->data);
            }
            return root;
        }

        bool searchInSubtree(Node* currentNode, const valueType& value) const {
            if (!currentNode) return false;
            if (value < currentNode->data) return searchInSubtree(currentNode->left, value);
            if (value > currentNode->data) return searchInSubtree(currentNode->right, value);
            return true;
        }
    };
} // namespace fsd::core

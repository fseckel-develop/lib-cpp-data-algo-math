// Created by Franz Seckel on 04.11.25.
#pragma once
#include <concepts>
#include "Deque.h"

template <typename C, typename T>
concept QueueContainer = requires(C container, const C constContainer, const T& value) {
    { container.pushBack(value) } -> std::same_as<void>;
    { container.popFront() } -> std::same_as<void>;
    { constContainer.front() } -> std::convertible_to<T>;
    { constContainer.empty() } -> std::convertible_to<bool>;
};

template <typename T, QueueContainer<T> C = Deque<T>>
class Queue {
public:
    // Aliases:
    using valueType = T;
    using constReference = const valueType&;
    using sizeType = size_t;
    using containerType = C;

    // Explicit Constructors:
    Queue() = default;
    explicit Queue(const containerType& container) : container_(container) {}
    explicit Queue(containerType&& container) noexcept(std::is_nothrow_move_constructible_v<containerType>) :
        container_(std::move(container)) {
    }

    // Capacity:
    sizeType size() const { return container_.size(); }
    bool empty() const { return container_.empty(); }

    // Element Access:
    constReference front() const { return container_.front(); }

    // Modifiers:
    void enqueue(const valueType& value) { container_.pushBack(value); }
    void dequeue() { container_.popFront(); }
    void swap(Queue& other) noexcept(std::is_nothrow_swappable_v<containerType>) {
        std::swap(container_, other.container_);
    }

private:
    containerType container_;
};

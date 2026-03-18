// Created by Franz Seckel on 03.11.25.
#pragma once
#include <concepts>
#include <fsd/core/vector.hpp>

namespace fsd::core
{
    template <typename C, typename T>
    concept StackContainer = requires(C container, const C constContainer, const T& value) {
        { container.pushBack(value) } -> std::same_as<void>;
        { container.popBack() } -> std::same_as<void>;
        { constContainer.back() } -> std::convertible_to<T>;
        { constContainer.empty() } -> std::convertible_to<bool>;
    };

    template <typename T, StackContainer<T> C = Vector<T>>
    class Stack {
    public:
        // Aliases:
        using valueType = T;
        using constReference = const valueType&;
        using sizeType = size_t;
        using containerType = C;

        // Explicit Constructors:
        Stack() = default;
        explicit Stack(const containerType& container) : container_(container) {}
        explicit Stack(containerType&& container) noexcept(std::is_nothrow_move_constructible_v<containerType>) :
            container_(std::move(container)) {
        }

        // Capacity:
        [[nodiscard]] sizeType size() const { return container_.size(); }
        [[nodiscard]] bool empty() const { return container_.empty(); }

        // Element Access:
        constReference top() const { return container_.back(); }

        // Modifiers:
        void push(const valueType& value) { container_.pushBack(value); }
        void pop() { container_.popBack(); }
        void swap(Stack& other) noexcept(std::is_nothrow_swappable_v<containerType>) {
            std::swap(container_, other.container_);
        }

    private:
        containerType container_;
    };
} // namespace fsd::core

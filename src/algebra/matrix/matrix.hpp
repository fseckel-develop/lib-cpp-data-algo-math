// Created by Franz Seckel on 19.12.2025.
#pragma once
#include <cassert>
#include <type_traits>
#include <initializer_list>
#include "matrix_storage.hpp"
#include "matrix_operations.hpp"

namespace fs::math::algebra
{
    /// @brief Fixed-size mathematical matrix type.
    ///
    /// Represents a row-major matrix with compile-time dimensions.
    /// The matrix stores its elements as an array of row vectors and
    /// provides arithmetic, indexing, and multiplication operations
    /// via CRTP-based mixins.
    ///
    /// Design properties:
    /// - Fixed size known at compile time
    /// - Trivially copyable if the value type is
    /// - Standard layout and SIMD-friendly
    /// - No virtual functions or dynamic allocation
    ///
    /// Storage:
    /// - Elements are stored as an array of row vectors
    /// - Accessed via operator[] returning a row vector
    ///
    /// Operations:
    /// - Provided by MatrixOperations via CRTP
    /// - Includes arithmetic, comparison, and multiplication
    ///
    /// @tparam valueType Scalar element type (must be arithmetic)
    /// @tparam Rows Number of rows (must be > 0)
    /// @tparam Columns Number of columns (must be > 0)
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    struct Matrix :
        /// @brief Storage mixin holding the raw row data.
        detail::MatrixStorage<valueType, Rows, Columns>,

        /// @brief CRTP mixin providing matrix operators and accessors.
        detail::MatrixOperations<Matrix<valueType, Rows, Columns>, valueType, Rows, Columns>
    {
        /// @brief Ensures matrix has at least one row.
        static_assert(Rows > 0, "Matrix rows count must be greater than zero");

        /// @brief Ensures matrix has at least one column.
        static_assert(Columns > 0, "Matrix columns count be greater than zero");

        /// @brief Ensures the matrix element type is arithmetic.
        static_assert(std::is_arithmetic_v<valueType>, "Matrix value type must be arithmetic");

        /// @brief Exposes the underlying row storage.
        /// This allows direct access to the row array for algorithms,
        /// interop, and low-level operations.
        using detail::MatrixStorage<valueType, Rows, Columns>::rows;

        /// @brief Default constructor
        constexpr Matrix() noexcept = default;

        /// @brief Constructs a matrix from a list of row vectors.
        /// @param list List of row vectors (must contain exactly Rows entries)
        constexpr Matrix(std::initializer_list<Vector<valueType, Columns>> list) noexcept
        {
            assert(list.size() == Rows && "Matrix row initializer list size mismatch");
            std::size_t row = 0;
            for (const auto& vector : list)
                rows[row++] = vector;
        }

        /// @brief Flat initializer constructor (row-major order)
        /// @param list List of Rows * Columns elements in row-major order
        constexpr Matrix(std::initializer_list<valueType> list) noexcept
        {
            assert(list.size() == Rows * Columns && "Flat initializer list size mismatch");
            auto it = list.begin();
            for (std::size_t row = 0; row < Rows; ++row)
                for (std::size_t column = 0; column < Columns; ++column)
                    rows[row][column] = *it++;
        }
    };
}

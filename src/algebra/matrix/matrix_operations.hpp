// Created by Franz Seckel on 19.12.2025.
#pragma once
#include "matrix_algorithms.hpp"
#include "matrix_fwd.hpp"

namespace fs::math::algebra::detail
{
    /// @brief CRTP mixin providing C++ operators and accessors for matrices.
    ///
    /// This type defines how matrices behave *syntactically*:
    /// - element access
    /// - arithmetic operators
    /// - comparisons
    /// - matrix/vector multiplication
    ///
    /// It intentionally does NOT implement mathematical algorithms
    /// such as inverse, transpose, determinant, etc.
    ///
    /// @tparam DerivedMatrix Concrete matrix type
    /// @tparam valueType Scalar type
    /// @tparam Rows Number of rows
    /// @tparam Columns Number of columns
    template<typename DerivedMatrix, typename valueType, std::size_t Rows, std::size_t Columns>
    struct MatrixOperations
    {
        /// @brief Mutable row access.
        /// @param row Row index
        /// @return Reference to row vector
        constexpr Vector<valueType, Columns>&
        operator[](std::size_t row) noexcept
        {
            return static_cast<DerivedMatrix*>(this)->rows[row];
        }

        /// @brief Const row access.
        /// @param row Row index
        /// @return Const reference to row vector
        constexpr const Vector<valueType, Columns>&
        operator[](std::size_t row) const noexcept
        {
            return static_cast<const DerivedMatrix*>(this)->rows[row];
        }

        /// @brief Returns a copy of a matrix row.
        /// @param row Row index
        /// @return Row vector (by value)
        [[nodiscard]] constexpr Vector<valueType, Columns>
        row(std::size_t row) const noexcept
        {
            return (*this)[row];
        }

        /// @brief Returns a column vector extracted from the matrix.
        /// @param column Column index
        /// @return Column vector (by value)
        [[nodiscard]] constexpr Vector<valueType, Rows>
        column(std::size_t column) const noexcept
        {
            Vector<valueType, Rows> result{};
            for (std::size_t row = 0; row < Rows; ++row)
                result[row] = (*this)[row][column];
            return result;
        }

        /// @brief In-place matrix addition.
        constexpr DerivedMatrix&
        operator+=(const DerivedMatrix& other) noexcept
        {
            for (std::size_t row = 0; row < Rows; ++row)
                (*this)[row] += other[row];
            return static_cast<DerivedMatrix&>(*this);
        }

        /// @brief Matrix addition.
        constexpr DerivedMatrix
        operator+(const DerivedMatrix& other) const noexcept
        {
            DerivedMatrix result = static_cast<const DerivedMatrix&>(*this);
            result += other;
            return result;
        }

        /// @brief In-place matrix subtraction.
        constexpr DerivedMatrix&
        operator-=(const DerivedMatrix& other) noexcept
        {
            for (std::size_t row = 0; row < Rows; ++row)
                (*this)[row] -= other[row];
            return static_cast<DerivedMatrix&>(*this);
        }

        /// @brief Matrix subtraction.
        constexpr DerivedMatrix
        operator-(const DerivedMatrix& other) const noexcept
        {
            DerivedMatrix result = static_cast<const DerivedMatrix&>(*this);
            result -= other;
            return result;
        }

        /// @brief Unary plus (identity).
        [[nodiscard]] constexpr DerivedMatrix operator+() const noexcept
        {
            return static_cast<const DerivedMatrix&>(*this);
        }

        /// @brief Unary negation.
        constexpr DerivedMatrix operator-() const noexcept
        {
            DerivedMatrix result{};
            for (std::size_t row = 0; row < Rows; ++row)
                result[row] = -(*this)[row];
            return result;
        }

        /// @brief In-place scalar multiplication.
        constexpr DerivedMatrix&
        operator*=(valueType scalar) noexcept
        {
            for (std::size_t row = 0; row < Rows; ++row)
                (*this)[row] *= scalar;
            return static_cast<DerivedMatrix&>(*this);
        }

        /// @brief Scalar multiplication.
        constexpr DerivedMatrix
        operator*(valueType scalar) const noexcept
        {
            DerivedMatrix result = static_cast<const DerivedMatrix&>(*this);
            result *= scalar;
            return result;
        }

        /// @brief In-place scalar division.
        constexpr DerivedMatrix&
        operator/=(valueType scalar) noexcept
        {
            for (std::size_t row = 0; row < Rows; ++row)
                (*this)[row] /= scalar;
            return static_cast<DerivedMatrix&>(*this);
        }

        /// @brief Scalar division.
        constexpr DerivedMatrix
        operator/(valueType scalar) const noexcept
        {
            DerivedMatrix result =static_cast<const DerivedMatrix&>(*this);
            result /= scalar;
            return result;
        }

        /// @brief Matrix-vector multiplication.
        /// @param vector Column vector
        /// @return Resulting vector
        [[nodiscard]] constexpr Vector<valueType, Rows>
        operator*(const Vector<valueType, Columns>& vector) const noexcept
        {
            return multiply(static_cast<const DerivedMatrix&>(*this), vector);
        }

        /// @brief Matrix-matrix multiplication.
        template<std::size_t OtherColumns>
        [[nodiscard]] constexpr Matrix<valueType, Rows, OtherColumns>
        operator*(const Matrix<valueType, Columns, OtherColumns>& other) const noexcept
        {
            return multiply(static_cast<const DerivedMatrix&>(*this), other);
        }

        /// @brief Deleted to prevent ambiguous semantics.
        template<std::size_t OtherColumns>
        constexpr DerivedMatrix&
        operator*=(const Matrix<valueType, Columns, OtherColumns>&) = delete;

        /// @brief Equality comparison.
        [[nodiscard]] constexpr bool
        operator==(const DerivedMatrix& other) const noexcept
        {
            for (std::size_t row = 0; row < Rows; ++row)
                if ((*this)[row] != other[row])
                    return false;
            return true;
        }

        /// @brief Inequality comparison.
        [[nodiscard]] constexpr bool
        operator!=(const DerivedMatrix& other) const noexcept
        {
            return !(*this == other);
        }
    };

    /// @brief Stream output operator for matrices.
    /// Prints the matrix row by row, one row per line.
    /// @param outputStream Output stream
    /// @param matrix Matrix to print
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    std::ostream&
    operator<<(std::ostream& outputStream, const Matrix<valueType, Rows, Columns>& matrix)
    {
        for (std::size_t row = 0; row < Rows; ++row)
        {
            outputStream << matrix[row];
            if (row + 1 < Rows) outputStream << '\n';
        }
        return outputStream;
    }

} // namespace fs::math::algebra::detail

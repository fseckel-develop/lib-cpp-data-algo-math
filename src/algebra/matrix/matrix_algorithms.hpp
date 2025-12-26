// Created by Franz Seckel on 20.12.2025.
#pragma once
#include <cmath>
#include "matrix_fwd.hpp"
#include "../vector/vector_algorithms.hpp"

namespace fs::math::algebra
{
    /// @brief Extracts a row vector from a matrix.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Vector<valueType, Columns>
    row(const Matrix<valueType, Rows, Columns>& matrix, std::size_t row) noexcept
    {
        return matrix[row];
    }

    /// @brief Extracts a column vector from a matrix.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Vector<valueType, Rows>
    column(const Matrix<valueType, Rows, Columns>& matrix, std::size_t column) noexcept {
        Vector<valueType, Rows> result{};
        for (std::size_t row = 0; row < Rows; ++row)
            result[row] = matrix[row][column];
        return result;
    }

    /// @brief Returns a zero-initialized matrix.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Matrix<valueType, Rows, Columns> zero() noexcept
    {
        return {};
    }

    /// @brief Returns an identity matrix.
    template<typename valueType, std::size_t Dimension>
    [[nodiscard]] constexpr Matrix<valueType, Dimension, Dimension> identity() noexcept
    {
        Matrix<valueType, Dimension, Dimension> result{};
        for (std::size_t i = 0; i < Dimension; ++i)
            result[i][i] = valueType{1};
        return result;
    }

    /// @brief Returns the transpose of a matrix.///
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Matrix<valueType, Columns, Rows>
    transpose(const Matrix<valueType, Rows, Columns>& matrix) noexcept
    {
        Matrix<valueType, Columns, Rows> result{};
        for (std::size_t row = 0; row < Rows; ++row)
            for (std::size_t column = 0; column < Columns; ++column)
                result[column][row] = matrix[row][column];
        return result;
    }

    /// @brief Transposes a square matrix in place.
    /// @warning Only valid for square matrices.
    template<typename valueType, std::size_t N>
    constexpr void transposeInPlace(Matrix<valueType, N, N>& matrix) noexcept
    {
        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = i + 1; j < N; ++j)
                std::swap(matrix[i][j], matrix[j][i]);
    }

    /// @brief Checks whether a matrix is identity within an epsilon.
    template<typename valueType, std::size_t N>
    [[nodiscard]] constexpr bool
    isIdentity(const Matrix<valueType, N, N>& matrix, valueType epsilon = valueType(1e-6)) noexcept
    {
        for (std::size_t row = 0; row < N; ++row)
            for (std::size_t column = 0; column < N; ++column)
            {
                const valueType expected = (row == column) ? valueType(1) : valueType(0);
                if (std::abs(matrix[row][column] - expected) > epsilon)
                    return false;
            }
        return true;
    }

    /// @brief Checks if a square matrix is orthogonal.
    /// Useful for validating rotation / view matrices.
    template<typename valueType, std::size_t N>
    [[nodiscard]] bool
    isOrthogonal(const Matrix<valueType, N, N>& matrix, valueType epsilon = valueType(1e-6)) noexcept
    {
        const auto matrixTranspose = transpose(matrix);
        const auto product = multiply(matrixTranspose, matrix);
        return isIdentity(product, epsilon);
    }

    /// @brief Matrix-vector multiplication.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Vector<valueType, Rows>
    multiply(const Matrix<valueType, Rows, Columns>& matrix, const Vector<valueType, Columns>& vector) noexcept
    {
        Vector<valueType, Rows> result{};
        for (std::size_t row = 0; row < Rows; ++row)
            result[row] = dot(matrix[row], vector);
        return result;
    }

    /// @brief Vector-matrix multiplication.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Vector<valueType, Columns>
    multiply(const Vector<valueType, Rows>& vector, const Matrix<valueType, Rows, Columns>& matrix) noexcept
    {
        Vector<valueType, Columns> result{};
        for (std::size_t column = 0; column < Columns; ++column)
            result[column] = dot(vector, column(matrix, column));
        return result;
    }


    /// @brief Matrix-matrix multiplication.
    template<typename valueType, std::size_t Rows, std::size_t Inner, std::size_t Columns>
    [[nodiscard]] constexpr Matrix<valueType, Rows, Columns>
    multiply(const Matrix<valueType, Rows, Inner>& first, const Matrix<valueType, Inner, Columns>& second) noexcept
    {
        Matrix<valueType, Rows, Columns> result{};
        for (std::size_t row = 0; row < Rows; ++row)
            for (std::size_t col = 0; col < Columns; ++col)
                result[row][col] = dot(first[row], column(second, col));
        return result;
    }

    /// @brief Element-wise (Hadamard) product of two matrices.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Matrix<valueType, Rows, Columns>
    multiplyHadamard(const Matrix<valueType, Rows, Columns>& first, const Matrix<valueType, Rows, Columns>& second) noexcept
    {
        Matrix<valueType, Rows, Columns> result{};
        for (std::size_t row = 0; row < Rows; ++row)
            for (std::size_t column = 0; column < Columns; ++column)
                result[row][column] = first[row][column] * second[row][column];
        return result;
    }


    /// @brief Computes the outer product of two vectors.
    /// Resulting matrix has dimensions Rows x Columns.///
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr Matrix<valueType, Rows, Columns>
    outerProduct(const Vector<valueType, Rows>& first, const Vector<valueType, Columns>& second) noexcept
    {
        Matrix<valueType, Rows, Columns> result{};
        for (std::size_t row = 0; row < Rows; ++row)
            for (std::size_t column = 0; column < Columns; ++column)
                result[row][column] = first[row] * second[column];
        return result;
    }

    /// @brief Computes the trace of a matrix.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] constexpr valueType trace(const Matrix<valueType, Rows, Columns>& matrix) noexcept
    {
        constexpr std::size_t N = (Rows < Columns) ? Rows : Columns;
        valueType sum{};
        for (std::size_t i = 0; i < N; ++i)
            sum += matrix[i][i];
        return sum;
    }

    /// @brief Computes the Frobenius norm of a matrix.
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    [[nodiscard]] valueType
    frobeniusNorm(const Matrix<valueType, Rows, Columns>& matrix) noexcept
    {
        valueType sum{};
        for (std::size_t row = 0; row < Rows; ++row)
            sum += dot(matrix[row], matrix[row]);
        return static_cast<valueType>(std::sqrt(sum));
    }

    /// @brief Determinant of a 2x2 matrix.
    template<typename valueType>
    [[nodiscard]] constexpr valueType
    determinant(const Matrix<valueType,2,2>& matrix) noexcept
    {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    /// @brief Determinant of a 3x3 matrix.
    template<typename valueType>
    [[nodiscard]] constexpr valueType  determinant(const Matrix<valueType,3,3>& matrix) noexcept
    {
        return
            matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
            matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
            matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    }

    /// @brief Inverse of a 2x2 matrix.
    /// @warning Undefined if determinant is zero.
    template<typename valueType>
    [[nodiscard]] Matrix<valueType,2,2> inverse(const Matrix<valueType,2,2>& matrix) noexcept
    {
        Matrix<valueType,2,2> result{};
        result[0][0] =  matrix[1][1];
        result[0][1] = -matrix[0][1];
        result[1][0] = -matrix[1][0];
        result[1][1] =  matrix[0][0];
        return result / determinant(matrix);
    }

    /// @brief Inverse of a 3x3 matrix.
    /// @warning Undefined if determinant is zero.
    template<typename valueType>
    [[nodiscard]] Matrix<valueType,3,3> inverse(const Matrix<valueType,3,3>& matrix) noexcept
    {
        Matrix<valueType,3,3> result{};
        result[0][0] =  (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
        result[0][1] = -(matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]);
        result[0][2] =  (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]);
        result[1][0] = -(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
        result[1][1] =  (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]);
        result[1][2] = -(matrix[0][0] * matrix[1][2] - matrix[0][2] * matrix[1][0]);
        result[2][0] =  (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
        result[2][1] = -(matrix[0][0] * matrix[2][1] - matrix[0][1] * matrix[2][0]);
        result[2][2] =  (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
        return result / determinant(matrix);
    }

    /// @brief Inverse of a 4x4 matrix.
    /// @note Optimized for graphics usage.
    /// @warning Undefined if determinant is zero.
    template<typename valueType>
    [[nodiscard]] Matrix<valueType,4,4> inverse(const Matrix<valueType,4,4>& matrix) noexcept
    {
        if (matrix[3] == Vector<valueType,4>{0,0,0,1})
            return inverseAffine(matrix);

        // Partial implementation — graphics-specific
        Matrix<valueType,4,4> result{};
        const valueType* a = &matrix[0][0];
        valueType* o = &result[0][0];

        o[0] = + a[5]*a[10]*a[15] - a[5]*a[11]*a[14]
               - a[9]*a[6]*a[15]  + a[9]*a[7]*a[14]
               + a[13]*a[6]*a[11] - a[13]*a[7]*a[10];

        const valueType det = a[0] * o[0];
        return result / det;
    }

    /// @brief Inverse of an affine 4x4 matrix.
    template<typename valueType>
    [[nodiscard]] Matrix<valueType,4,4> inverseAffine(const Matrix<valueType,4,4>& matrix) noexcept
    {
        const Matrix<valueType,3,3> rotationScale {
            { matrix[0][0], matrix[0][1], matrix[0][2] },
            { matrix[1][0], matrix[1][1], matrix[1][2] },
            { matrix[2][0], matrix[2][1], matrix[2][2] }
        };

        const Matrix<valueType,3,3> rotationScaleInverse = inverse(rotationScale);
        const Vector<valueType,3> translation { matrix[0][3], matrix[1][3], matrix[2][3] };
        const Vector<valueType,3> translationInverse = -(rotationScaleInverse * translation);

        Matrix<valueType,4,4> result = identity<valueType,4>();
        for (std::size_t row = 0; row < 3; ++row)
            for (std::size_t column = 0; column < 3; ++column)
                result[row][column] = rotationScaleInverse[row][column];

        result[0][3] = translationInverse.x();
        result[1][3] = translationInverse.y();
        result[2][3] = translationInverse.z();

        return result;
    }

} // namespace fs::math::algebra

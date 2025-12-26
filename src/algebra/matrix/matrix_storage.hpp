// Created by Franz Seckel on 19.12.2025.
#pragma once
#include <cstddef>
#include "algebra/vector/vector_fwd.hpp"

namespace fs::math::algebra::detail
{
    /// @brief Low-level storage for a fixed-size matrix.
    ///
    /// This type defines the **exact memory layout** of a matrix.
    ///
    /// Design guarantees:
    /// - Row-major storage
    /// - Contiguous memory
    /// - Trivially copyable
    /// - Standard-layout type
    /// - No constructors or destructors
    ///
    /// Responsibilities:
    /// - Owns the raw matrix data
    /// - Nothing else
    ///
    /// Non-responsibilities:
    /// - No bounds checking
    /// - No arithmetic
    /// - No algorithms
    /// - No alignment tricks (handled elsewhere if needed)
    ///
    /// This struct is intentionally minimal and is meant to be
    /// composed into higher-level matrix types via inheritance.
    ///
    /// @tparam valueType Scalar type stored in the matrix
    /// @tparam Rows Number of rows
    /// @tparam Columns Number of columns
    template<typename valueType, std::size_t Rows, std::size_t Columns>
    struct MatrixStorage
    {
        /// @brief Array of row vectors.
        /// Each element represents one row of the matrix.
        /// Rows are stored contiguously in memory.
        Vector<valueType, Columns> rows[Rows];

        /// @brief Zero-initialize all matrix elements
        constexpr MatrixStorage() noexcept :
            rows{} {
        }
    };

} // namespace fs::math::algebra::detail

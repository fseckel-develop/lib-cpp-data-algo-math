// Created by Franz Seckel on 20.12.2025.
#pragma once
#include "matrix.hpp"

namespace fs::math::algebra
{
    using Matrix2 = Matrix<float,2,2>;
    using Matrix3 = Matrix<float,3,3>;
    using Matrix4 = Matrix<float,4,4>;

    using Matrix2d = Matrix<double,2,2>;
    using Matrix3d = Matrix<double,3,3>;
    using Matrix4d = Matrix<double,4,4>;

    using Matrix2i = Matrix<int,2,2>;
    using Matrix3i = Matrix<int,3,3>;
    using Matrix4i = Matrix<int,4,4>;

} // namespace fs::math::algebra

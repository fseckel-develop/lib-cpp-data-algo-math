// Created by Franz Seckel on 20.12.2025.
#pragma once
#include <cmath>
#include "matrix_aliases.hpp"
#include "algebra/vector/vector_aliases.hpp"
#include "algebra/vector/vector_algorithms.hpp"

namespace fs::math::algebra
{
    /// @brief Creates a 4x4 translation matrix.
    /// Uses row-major layout with column vectors on the right.
    /// @param t Translation vector
    /// @return Translation matrix
    [[nodiscard]] inline Matrix4
    translate(const Vector3& t) noexcept
    {
        const Matrix4 result = {
            { 1.0f, 0.0f, 0.0f, t.x() },
            { 0.0f, 1.0f, 0.0f, t.y() },
            { 0.0f, 0.0f, 1.0f, t.z() },
            { 0.0f, 0.0f, 0.0f, 1.0f  }
        };
        return result;
    }

    /// @brief Creates a 4x4 scaling matrix.
    /// @param s Scale factors along x, y and z axes
    /// @return Scaling matrix
    [[nodiscard]] inline Matrix4
    scale(const Vector3& s) noexcept
    {
        const Matrix4 result ={
            { s.x(),  0.0f,  0.0f, 0.0f },
            {  0.0f, s.y(),  0.0f, 0.0f },
            {  0.0f,  0.0f, s.z(), 0.0f },
            {  0.0f,  0.0f,  0.0f, 1.0f }
        };
        return result;
    }

    /// @brief Creates a rotation matrix around an arbitrary axis.
    /// Uses Rodrigues' rotation formula.
    /// @param axis Rotation axis (will be normalized)
    /// @param angleRadians Rotation angle in radians
    /// @return Rotation matrix
    [[nodiscard]] inline Matrix4
    rotate(const Vector3& axis, const float angleRadians) noexcept
    {
        const Vector3 a = normalize(axis);
        const float c = std::cos(angleRadians);
        const float s = std::sin(angleRadians);
        const float t = 1.0f - c;

        const Matrix4 result = {
            {         t * a.x() * a.x() + c, t * a.x() * a.y() - s * a.z(), t * a.x() * a.z() + s * a.y(), 0.0f },
            { t * a.x() * a.y() + s * a.z(),         t * a.y() * a.y() + c, t * a.y() * a.z() - s * a.x(), 0.0f },
            { t * a.x() * a.z() - s * a.y(), t * a.y() * a.z() + s * a.x(),         t * a.z() * a.z() + c, 0.0f },
            {                          0.0f,                          0.0f,                          0.0f, 1.0f }
        };
        return result;
    }

    /// @brief Creates a rotation matrix around the X axis.
    [[nodiscard]] inline Matrix4 rotateX(const float angleRadians) noexcept
    {
        const float c = std::cos(angleRadians);
        const float s = std::sin(angleRadians);

        const Matrix4 result = {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f,    c,   -s, 0.0f },
            { 0.0f,    s,    c, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return result;
    }

    /// @brief Creates a rotation matrix around the Y axis.
    [[nodiscard]] inline Matrix4 rotateY(const float angleRadians) noexcept
    {
        const float c = std::cos(angleRadians);
        const float s = std::sin(angleRadians);

        const Matrix4 result = {
            {    c, 0.0f,    s, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            {   -s, 0.0f,    c, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return result;
    }

    /// @brief Creates a rotation matrix around the Z axis.
    [[nodiscard]] inline Matrix4 rotateZ(const float angleRadians) noexcept
    {
        const float c = std::cos(angleRadians);
        const float s = std::sin(angleRadians);

        const Matrix4 result = {
            {    c,   -s, 0.0f, 0.0f },
            {    s,    c, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return result;
    }

} // namespace fs::math::algebra

// Created by Franz Seckel on 20.12.2025.
#pragma once
#include <cmath>
#include "matrix_aliases.hpp"

namespace fs::math::algebra
{
    /// @brief Creates a right-handed view (camera) matrix.
    ///
    /// Builds a look-at matrix using a right-handed coordinate system and
    /// row-major layout. The resulting matrix transforms world-space coordinates
    /// into view (camera) space.
    ///
    /// Conventions:
    /// - Forward direction points from @p eye to @p center
    /// - Right vector is computed as cross(forward, up)
    /// - Translation is stored in the last column
    ///
    /// @param eye Camera position in world space
    /// @param center Target position the camera looks at
    /// @param up Approximate world up direction
    /// @return View matrix
    inline Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) noexcept
    {
        const Vector3 f = normalize(center - eye);
        const Vector3 s = normalize(cross(f, up));
        const Vector3 u = cross(s, f);

        const Matrix4 result = {
            {  s.x(),  s.y(),  s.z(), -dot(s, eye) },
            {  u.x(),  u.y(),  u.z(), -dot(u, eye) },
            { -f.x(), -f.y(), -f.z(),  dot(f, eye) },
            {   0.0f,   0.0f,   0.0f,         1.0f }
        };
        return result;
    }

    /// @brief Creates a perspective projection matrix.
    ///
    /// Produces a right-handed perspective projection matrix compatible with
    /// OpenGL-style normalized device coordinates:
    /// - Clip space Z range: [-1, 1]
    /// - Camera looks down the negative Z axis
    ///
    /// @param fovRadians Vertical field of view in radians
    /// @param aspect Aspect ratio (width / height)
    /// @param zNear Distance to near clipping plane (zNear > 0)
    /// @param zFar Distance to far clipping plane (zFar > zNear)
    /// @return Perspective projection matrix
    inline Matrix4 perspective(const float fovRadians, const float aspect, const float zNear, const float zFar) noexcept
    {
        const float f = 1.0f / std::tan(fovRadians * 0.5f);

        const Matrix4 result = {
            { f / aspect, 0.0f,                            0.0f,                                 0.0f },
            {       0.0f,    f,                            0.0f,                                 0.0f },
            {       0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), 2.0f * zFar * zNear / (zNear - zFar) },
            {       0.0f, 0.0f,                           -1.0f,                                 0.0f }
        };
        return result;
    }

    /// @brief Creates an orthographic projection matrix.
    ///
    /// Produces a right-handed orthographic projection matrix with
    /// OpenGL-style clip space:
    /// - X and Y mapped to [-1, 1]
    /// - Z mapped to [-1, 1]
    ///
    /// @param left Left clipping plane
    /// @param right Right clipping plane
    /// @param bottom Bottom clipping plane
    /// @param top Top clipping plane
    /// @param zNear Near clipping plane
    /// @param zFar Far clipping plane
    /// @return Orthographic projection matrix
    inline Matrix4 orthographic(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar) noexcept
    {
        const Matrix4 result = {
            { 2.0f / (right - left),                  0.0f,                   0.0f, -(right + left) / (right - left) },
            {                  0.0f, 2.0f / (top - bottom),                   0.0f, -(top + bottom) / (top - bottom) },
            {                  0.0f,                  0.0f, -2.0f / (zFar - zNear), -(zFar + zNear) / (zFar - zNear) },
            {                  0.0f,                  0.0f,                   0.0f,                             1.0f }
        };
        return result;
    }

} // namespace fs::math::algebra

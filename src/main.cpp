// Created by Franz Seckel on 17.12.2025.
#include <iostream>
#include "algebra/vector.hpp"
#include "algebra/matrix.hpp"

using namespace fs::math::algebra;

int main() {
    constexpr Vector3 translation = {1.0f, 2.0f, 3.0f};
    constexpr Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    constexpr Vector3 scaleFactors = {3.0f, 4.0f, 0.5f};
    std::cout << "Translation:   " << translation << std::endl;
    std::cout << "Rotation axis: " << rotationAxis << std::endl;
    std::cout << "Scale along X: " << scaleFactors.x() << std::endl;
    std::cout << "Scale along Y: " << scaleFactors.y() << std::endl;
    std::cout << "Scale along Z: " << scaleFactors.z() << std::endl;

    const auto matrix = translate(translation) * rotate(rotationAxis, 45.0);
    std::cout << "Matrix:\n" << matrix << std::endl;
}

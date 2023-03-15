#pragma once
#include "mat3x3.hpp"
#include "vector.hpp"

#include <cmath>
#include <limits>
#include <random>


namespace math {
    constexpr float MATH_EPSILON = std::numeric_limits<float>::epsilon();
    constexpr float MATH_PI = 3.14159265359f;
    constexpr float MATH_INFINITY = std::numeric_limits<float>::infinity();

    inline float ToDegrees(float radians) noexcept {
        return (180.0f * radians) / MATH_PI;
    }
    
    inline float ToRadians(float degrees) noexcept {
        return degrees * (MATH_PI / 180.0f);
    }

    inline float Random() noexcept {
        static std::uniform_real_distribution<float> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    template<typename VecType, typename MatrixType>
    inline auto Mult(const Vector<VecType, 3>& vec, const Mat3x3<MatrixType>& mat) noexcept -> Vector<decltype(vec.x * mat[0][0]), 3> {
        return Vector<decltype(vec.x * mat[0][0]), 3>(
            vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0],
            vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1],
            vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2]
        );
    }

    template<typename Type>
    inline bool IsTendsTo(const Type& value, const Type& limit) noexcept {
        return std::abs(limit - value) <= MATH_EPSILON;
    }

    template <typename Type>
    Type Clamp(Type value, Type min, Type max) noexcept {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic value");
        if (min > max) {
            std::swap(min, max);
        }

        return (value > max) ? max : (value < min) ? min : value;
    }
}
#pragma once
#include "matrix_operations.hpp"
#include "vector_operations.hpp"

#include "const.hpp"

#include <cmath>
#include <random>

namespace math {
    constexpr float ToDegrees(float radians) noexcept {
        return (180.0f * radians) / MATH_PI;
    }

    constexpr float ToRadians(float degrees) noexcept {
        return degrees * (MATH_PI / 180.0f);
    }

    template <typename Type>
    inline constexpr Type Abs(Type value) noexcept {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type>"
            "inline constexpr Type Abs(Type value) noexcept: Type must be an arithmetic"
        );
        return value > Type(0) ? value : -value;
    }

    template <typename Type>
    Type Random(Type min, Type max) noexcept {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type>"
            "Type Random(Type min, Type max) noexcept: Type must be an arithmetic"
        );
        
        static std::random_device _rd;
        static std::mt19937 gen(_rd());

        if constexpr (std::is_floating_point_v<Type>) {
            return std::uniform_real_distribution<Type>(min, max)(gen);
        } else {
            return std::uniform_int_distribution<Type>(min, max)(gen);
        }
    }

    template<typename Type1, typename Type2>
    inline constexpr bool IsTendsTo(Type1 value, Type2 limit) noexcept {
        static_assert(std::is_arithmetic_v<Type1>, "template<typename Type1, typename Type2>"
            "inline constexpr bool IsTendsTo(Type1 value, Type2 limit) noexcept: Type1 must be an arithmetic"
        );
        static_assert(std::is_arithmetic_v<Type2>, "template<typename Type1, typename Type2>"
            "inline constexpr bool IsTendsTo(Type1 value, Type2 limit) noexcept: Type2 must be an arithmetic"
        );

        return Abs(limit - value) <= MATH_EPSILON;
    }

    template <typename Type>
    inline constexpr Type Clamp(Type value, Type min, Type max) noexcept {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type>"
            "inline constexpr Type Clamp(Type value, Type min, Type max) noexcept: Type must be an arithmetic"
        );
        return (value > max) ? max : (value < min) ? min : value;
    }
}
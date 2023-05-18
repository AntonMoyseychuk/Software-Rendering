#pragma once
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

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    inline constexpr Type Abs(Type value) noexcept {
        return value >= Type(0) ? value : -value;
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    Type Random(Type min, Type max) noexcept {       
        static std::random_device _rd;
        static std::mt19937 gen(_rd());

        if constexpr (std::is_floating_point_v<Type>) {
            return std::uniform_real_distribution<Type>(min, max)(gen);
        } else {
            return std::uniform_int_distribution<Type>(min, max)(gen);
        }
    }

    template<typename Type1, typename Type2, typename = std::enable_if_t<std::is_arithmetic_v<Type1>>, typename = std::enable_if_t<std::is_arithmetic_v<Type2>>>
    inline constexpr bool IsTendsTo(Type1 value, Type2 limit) noexcept {
        return Abs(limit - value) <= MATH_EPSILON;
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    inline constexpr Type Clamp(Type value, Type min, Type max) noexcept {
        return (value > max) ? max : (value < min) ? min : value;
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    inline std::vector<Type>& Interpolate(Type i0, Type d0, Type i1, Type d1, std::vector<Type>& values) noexcept {
        if (i0 == i1) {
            values.resize(1, d0);
            return values;
        }

        values.resize(i1 - i0 + 1);
        
        const float a = static_cast<float>(d1 - d0) / (i1 - i0);
        for (size_t i = 0; i <= size_t(i1 - i0); ++i) {
            values[i] = static_cast<Type>(d0);
            d0 += a;
        }

        return values;
    }
}
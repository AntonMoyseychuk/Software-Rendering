#pragma once
#include "const.hpp"

#include "mat4.hpp"
#include "mat_operations.hpp"

#include "quaternion.hpp"

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "vec_operations.hpp"

#include <cmath>
#include <random>

namespace math {
    constexpr float to_degrees(float radians) noexcept {
        return (180.0f * radians) / MATH_PI;
    }

    constexpr float to_radians(float degrees) noexcept {
        return degrees * (MATH_PI / 180.0f);
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    inline constexpr Type abs(Type value) noexcept {
        return value >= Type(0) ? value : -value;
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    Type random(Type min, Type max) noexcept {       
        static std::random_device _rd;
        static std::mt19937 gen(_rd());

        if constexpr (std::is_floating_point_v<Type>) {
            return std::uniform_real_distribution<Type>(min, max)(gen);
        } else {
            return std::uniform_int_distribution<Type>(min, max)(gen);
        }
    }

    template<typename Type1, typename Type2, typename = std::enable_if_t<std::is_arithmetic_v<Type1>>, typename = std::enable_if_t<std::is_arithmetic_v<Type2>>>
    inline constexpr bool is_tends_to(Type1 value, Type2 limit) noexcept {
        return abs(limit - value) <= MATH_EPSILON;
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    inline constexpr Type clamp(Type value, Type min, Type max) noexcept {
        return (value > max) ? max : (value < min) ? min : value;
    }

    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    inline std::vector<Type>& interpolate(Type i0, Type d0, Type i1, Type d1, std::vector<Type>& values) noexcept {
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
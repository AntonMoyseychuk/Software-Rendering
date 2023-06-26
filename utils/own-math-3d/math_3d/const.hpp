#pragma once

#include <limits>

namespace math {
    constexpr float MATH_INFINITY = std::numeric_limits<float>::infinity();
    constexpr float MATH_EPSILON = std::numeric_limits<float>::epsilon();
    
    constexpr float MATH_PI = 3.14159265359f;
    constexpr float MATH_PI_MUL_2 = MATH_PI * 2.0f;
    constexpr float MATH_PI_DIV_2 = MATH_PI / 2.0f;
    constexpr float MATH_PI_DIV_4 = MATH_PI / 4.0f;
}
#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    struct Ray {
        Ray() = default;
        Ray(const math::vec4f& original, const math::vec4f& direction);

        math::vec4f original;
        math::vec4f direction;
    };
}
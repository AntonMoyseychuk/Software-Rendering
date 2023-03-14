#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    struct Ray {
        Ray() = default;
        Ray(const math::vec3f& original, const math::vec3f& direction);

        math::vec3f origin;
        math::vec3f direction;
    };
}
#pragma once
#include "math_3d/vec3.hpp"

namespace raytracing::gfx {
    struct Ray {
        Ray() = default;
        Ray(const math::vec3& original, const math::vec3& direction);

        math::vec3 origin;
        math::vec3 direction;
    };
}
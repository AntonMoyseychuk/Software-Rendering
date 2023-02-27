#pragma once
#include "vector.hpp"

namespace math {
    struct Ray {
        Vec3f original;
        Vec3f direction;

        Ray() = default;
        Ray(const Vec3f& original, const Vec3f& direction);
    };
}
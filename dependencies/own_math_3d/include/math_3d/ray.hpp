#pragma once
#include "vector3.hpp"

namespace math {
    struct Ray {
        Vector3f original;
        Vector3f direction;

        Ray() = default;
        Ray(const Vector3f& original, const Vector3f& direction);
    };
}
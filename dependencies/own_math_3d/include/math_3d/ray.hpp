#pragma once
#include "vector.hpp"

namespace math {
    struct Ray {
        Vector<float> original;
        Vector<float> direction;

        Ray() = default;
        Ray(const Vector<float>& original, const Vector<float>& direction);
    };
}
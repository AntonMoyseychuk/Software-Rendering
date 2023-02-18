#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "declarations.hpp"

namespace math {
    float Dot(const Vector3& a, const Vector3& b) noexcept;
    Vector3 Cross(const Vector3& a, const Vector3& b) noexcept;
    
    float Clamp(float left_border, float right_border, float value) noexcept;
}

#endif
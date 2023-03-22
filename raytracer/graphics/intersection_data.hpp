#pragma once
#include "math_3d/vector.hpp"
#include "material.hpp"

namespace gfx {
    struct IntersectionData {
        IntersectionData() = default;
        IntersectionData(const math::vec3f& _point, const math::vec3f& _normal, const Material& _material, float _distance) 
            : point(_point), normal(_normal), material(_material), distance(_distance) {}

        math::vec3f point;
        math::vec3f normal;
        Material material;

        float distance;
    };
}
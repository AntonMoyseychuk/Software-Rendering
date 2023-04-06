#pragma once
#include "ray.hpp"
#include "materials/material.hpp"

namespace gfx {
    struct IntersectionData {
        IntersectionData() = default;
        IntersectionData(
            const math::vec3f& _point, 
            const math::vec3f& _normal, 
            const gfx::Ray& _casted_ray, 
            float _distance, 
            const Material& _material
        ) : point(_point), normal(_normal), casted_ray(_casted_ray), distance(_distance), material(_material) {}

        math::vec3f point;
        math::vec3f normal;
        gfx::Ray casted_ray;
        float distance;

        Material material;
    };
}
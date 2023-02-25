#pragma once
#include "math_3d/vector3.hpp"

namespace shape {
    struct Sphere {
        Sphere() = default;
        Sphere(const math::Vector3f& position, float radius);

        bool IsRayIntersect(const math::Vector3f& ray_orig, const math::Vector3f& ray_dir) const noexcept;

        math::Vector3f position;
        float radius = 0.0f;
    };
    
}
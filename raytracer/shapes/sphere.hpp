#pragma once
#include "math_3d/vector3.hpp"
#include "math_3d/ray.hpp"

#include "../graphics/material.hpp"

namespace shape {
    struct Sphere {
        Sphere() = default;
        Sphere(const math::Vector3f& position, float radius, const gfx::Material& material);

        bool IsIntersect(const math::Ray& ray) const noexcept;
        math::Vector3f GetNormalAtPoint(const math::Vector3f& point) const noexcept;

        math::Vector3f position;
        float radius = 0.0f;
        gfx::Material material;
    };
    
}
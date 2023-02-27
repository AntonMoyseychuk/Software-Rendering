#pragma once
#include "math_3d/vector.hpp"
#include "math_3d/ray.hpp"

#include "../graphics/material.hpp"

namespace shape {
    struct Sphere {
        Sphere() = default;
        Sphere(const math::Vec3f& position, float radius, const gfx::Material& material);

        bool IsIntersect(const math::Ray& ray) const noexcept;
        math::Vec3f GetNormalAtPoint(const math::Vec3f& point) const noexcept;

        math::Vec3f position;
        float radius = 0.0f;
        gfx::Material material;
    };
    
}
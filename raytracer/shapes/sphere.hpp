#pragma once
#include "math_3d/vector.hpp"
#include "math_3d/ray.hpp"

#include "../graphics/material.hpp"

namespace shape {
    struct Sphere {
        Sphere() = default;
        Sphere(const math::Vector<float>& position, float radius, const gfx::Material& material);

        bool IsIntersect(const math::Ray& ray) const noexcept;

        math::Vector<float> position;
        float radius = 0.0f;
        gfx::Material material;
    };
    
}
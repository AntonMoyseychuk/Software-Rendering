#pragma once
#include "math_3d/vector.hpp"
#include "graphics/ray.hpp"

#include "graphics/material.hpp"

namespace gfx {
    struct Sphere {
        Sphere() = default;
        Sphere(const math::Vector<float>& position, float radius, const Material& material);

        bool IsIntersect(const Ray& ray) const noexcept;

        math::Vector<float> position;
        float radius = 0.0f;
        gfx::Material material;
    };
    
}
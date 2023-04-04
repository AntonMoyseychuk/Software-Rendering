#pragma once
#include "graphics/material.hpp"
#include "math_3d/vector.hpp"

#include "memory"

namespace gfx {
    struct Vertex {
        Vertex() = default;
        Vertex(const math::vec3f& _position, std::shared_ptr<Material> _material = nullptr)
            : position(_position), material(_material) {}

        math::vec3f position;
        std::shared_ptr<Material> material;
    };
}
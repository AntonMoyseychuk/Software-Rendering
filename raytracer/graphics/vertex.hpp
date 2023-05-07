#pragma once
#include "math_3d/vector.hpp"

#include <memory>

namespace raytracing::gfx {
    struct Vertex {
        Vertex() = default;
        Vertex(const math::vec3f& _position)
            : position(_position) {}

        math::vec3f position;
    };
}
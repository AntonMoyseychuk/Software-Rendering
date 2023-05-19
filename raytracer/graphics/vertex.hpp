#pragma once
#include "math_3d/vec3.hpp"

#include <memory>

namespace raytracing::gfx {
    struct Vertex {
        Vertex() = default;
        Vertex(const math::vec3& position)
            : position(position) {}

        math::vec3 position;
    };
}
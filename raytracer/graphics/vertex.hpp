#pragma once
#include "color.hpp"
#include "math_3d/vector.hpp"

#include <memory>

namespace gfx {
    struct Vertex {
        Vertex() = default;
        Vertex(const math::vec3f& _position, std::shared_ptr<Color> _color = nullptr)
            : position(_position), color(_color) {}

        math::vec3f position;
        std::shared_ptr<Color> color;
    };
}
#pragma once
#include "color.hpp"

namespace gfx {
    struct Material {
        Material() = default;
        Material(const Color& color, float specular_index);

        Color color = Color::BLACK;
        float specular_index = 0.0f;
    };
}
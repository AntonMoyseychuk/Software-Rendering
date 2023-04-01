#pragma once
#include "color.hpp"

namespace gfx {
    struct Material {
        Material(const Color& color = Color::BLACK, float specular_index = 0.0f, float reflective_index = 0.0f);

        Color color;
        float specular_index;
        float reflective_index;
    };
}
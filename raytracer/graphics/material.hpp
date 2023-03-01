#include "color.hpp"

namespace gfx {
    struct Material {
        Material() = default;
        Material(const Color& color, float refractive_index);

        Color color;
        float refractive_index = 0;
    };
}
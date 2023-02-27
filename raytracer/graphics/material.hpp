#include "math_3d/color.hpp"

namespace gfx {
    struct Material {
        Material() = default;
        Material(const math::Color& color, float refractive_index);

        math::Color color;
        float refractive_index = 0;
    };
}
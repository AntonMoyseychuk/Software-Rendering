#include "material.hpp"

namespace gfx {
    Material::Material(const math::Color &c, float r)
        : color(c), refractive_index(r)
    {
    }
}
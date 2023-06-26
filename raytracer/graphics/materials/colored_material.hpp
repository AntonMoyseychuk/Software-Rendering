#pragma once
#include "material.hpp"

namespace raytracing::gfx {
    struct IColoredMaterial : virtual public IMaterial {
        IColoredMaterial() = default;
        IColoredMaterial(const math::color& _color) : color(_color) {}

        math::color color;
    };
}
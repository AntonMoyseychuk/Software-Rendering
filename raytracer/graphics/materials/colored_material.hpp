#pragma once
#include "material.hpp"

namespace raytracing::gfx {
    struct IColoredMaterial : virtual public IMaterial {
        IColoredMaterial() = default;
        IColoredMaterial(math::Color _color) : color(_color) {}

        math::Color color;
    };
}
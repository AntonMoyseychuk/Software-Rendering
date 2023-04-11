#pragma once
#include "material.hpp"

namespace gfx {
    struct IColoredMaterial : virtual public IMaterial {
        IColoredMaterial() = default;
        IColoredMaterial(Color _color) : color(_color) {}

        Color color;
    };
}
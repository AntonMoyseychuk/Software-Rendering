#pragma once
#include "material.hpp"

namespace gfx {
    struct IGlaringMaterial : virtual public IMaterial {
        IGlaringMaterial() = default;
        IGlaringMaterial(float _specular_index) : IMaterial(), specular_index(_specular_index) {}

        float specular_index = 0.0f;
    };
}
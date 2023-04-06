#pragma once
#include "colored_material.hpp"

namespace gfx {
    class Metal : public IColoredMaterial {
    public:
        Metal() = default;
        Metal(Color color);

        bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept override;
    };
}
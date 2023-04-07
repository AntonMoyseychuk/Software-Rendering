#pragma once
#include "colored_material.hpp"

namespace gfx {
    class Diffuse : public IColoredMaterial {
    public:
        Diffuse() = default;
        Diffuse(Color color);
        bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept override;        
    };
}
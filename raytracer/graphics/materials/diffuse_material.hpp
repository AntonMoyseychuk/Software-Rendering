#pragma once
#include "colored_material.hpp"

namespace raytracing::gfx {
    struct Diffuse : public IColoredMaterial {
        Diffuse() = default;
        Diffuse(math::Color color);
        
        bool Scatter(const IntersectionData& int_data, math::Color& attenuation, Ray& scattered_ray) const noexcept override;        
    };
}
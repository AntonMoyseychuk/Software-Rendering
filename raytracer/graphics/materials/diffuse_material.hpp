#pragma once
#include "colored_material.hpp"

namespace raytracing::gfx {
    struct Diffuse : public IColoredMaterial {
        Diffuse() = default;
        Diffuse(const math::color& color);
        
        bool Scatter(const IntersectionData& int_data, math::color& attenuation, Ray& scattered_ray) const noexcept override;        
    };
}
#pragma once
#include "colored_material.hpp"
#include "glaring_material.hpp"

namespace raytracing::gfx {
    struct Metal : public IColoredMaterial, public IGlaringMaterial {
        Metal() = default;
        Metal(math::Color color, float reflection_index = 0.0f, float specular_index = 0.0f);

        bool Scatter(const IntersectionData& int_data, math::Color& attenuation, Ray& scattered_ray) const noexcept override;

        float reflection_index;
    };
}
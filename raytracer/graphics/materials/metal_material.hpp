#pragma once
#include "colored_material.hpp"

namespace gfx {
    class Metal : public IColoredMaterial {
    public:
        Metal() = default;
        Metal(Color color, float reflection_index = 0.0f);

        bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept override;

        void SetReflectionIndex(float reflection_index) noexcept;
        float GetReflectionIndex() noexcept;

    protected:
        float m_reflection_index;
    };
}
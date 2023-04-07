#pragma once
#include "material.hpp"

namespace gfx {
    class Dielectric : public IMaterial {
    public:
        Dielectric() = default;
        Dielectric(float refraction_index);

        bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept override;

        void SetRefractionIndex(float refraction_index) noexcept;
        float GetRefractionIndex() noexcept;

    protected:
        float m_refraction_index = 0.0f;
    };
}
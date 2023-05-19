#pragma once
#include "material.hpp"

namespace raytracing::gfx {
    struct Dielectric : public IMaterial {
        Dielectric() = default;
        Dielectric(float refraction_index);

        bool Scatter(const IntersectionData& int_data, math::color& attenuation, Ray& scattered_ray) const noexcept override;

    protected:
        static float _Reflectance(float cosine, float refraction_ratio) noexcept;

    public:
        float refraction_index = 0.0f;
    };
}
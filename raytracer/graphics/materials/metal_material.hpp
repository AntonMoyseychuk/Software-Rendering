#pragma once
#include "colored_material.hpp"

namespace gfx {
    class Metal : public IColoredMaterial {
    public:
        Metal() = default;
        Metal(Color color, float fuzz_index = 0.0f);

        bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept override;

        void SetFuzzIndex(float fuzz_index) noexcept;
        float GetFuzzIndex() noexcept;

    protected:
        float m_fuzz_index;
    };
}
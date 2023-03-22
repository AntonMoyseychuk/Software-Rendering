#pragma once
#include "light.hpp"

namespace gfx {
    class AmbientLight : public ILight {
    public:
        AmbientLight() = default;
        AmbientLight(const gfx::Color& color, float intensity);

        bool ComputeIllumination(const IntersectionData& int_data,
            gfx::Color& out_light_color, float& out_intensity) const noexcept override;
    };
}
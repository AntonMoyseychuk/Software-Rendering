#pragma once
#include "light.hpp"

namespace gfx {
    class AmbientLight : public ILight {
    public:
        AmbientLight() = default;
        AmbientLight(const gfx::Color& color, float intensity);

        bool ComputeIllumination(const IntersectionData& int_data, const math::vec3f& camera_pos, 
            gfx::Color& light_color, float& intensity) const noexcept override;
    };
}
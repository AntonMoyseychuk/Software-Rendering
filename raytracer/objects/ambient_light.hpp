#pragma once
#include "light.hpp"

namespace gfx {
    class AmbientLight : public ILight {
    public:
        AmbientLight() = default;
        AmbientLight(const gfx::Color& color, float intensity);

        bool ComputeIllumination(const math::vec3f& at_point, const math::vec3f& at_normal, 
            gfx::Color& color, float& intensity) const noexcept override;
    };
}
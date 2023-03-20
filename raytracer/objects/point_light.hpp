#pragma once
#include "light.hpp"

namespace gfx {
    class PointLigth : public ILight {
    public:
        PointLigth() = default;
        PointLigth(const math::vec3f& position, const gfx::Color& color, float intensity);
        ~PointLigth() override;

        bool ComputeIllumination(const math::vec3f& at_point, const math::vec3f& at_normal,
            gfx::Color& light_color, float& intensity) const noexcept override;
    };
}
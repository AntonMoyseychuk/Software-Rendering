#pragma once
#include "light.hpp"

namespace raytracing::gfx {
    class PointLigth : public ILight {
    public:
        PointLigth() = default;
        PointLigth(const math::vec3f& position, math::Color color, float intensity);

        bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept override;

        void MoveFor(const math::vec3f& dist) noexcept;


        math::vec3f position;
    };
}
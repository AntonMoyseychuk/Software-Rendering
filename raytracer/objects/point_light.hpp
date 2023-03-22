#pragma once
#include "light.hpp"

namespace gfx {
    class PointLigth : public ILight {
    public:
        PointLigth() = default;
        PointLigth(const math::vec3f& position, const gfx::Color& color, float intensity);

        void MoveFor(const math::vec3f& dist) noexcept;
        
        void SetPositon(const math::vec3f& position) noexcept;
        const math::vec3f& GetPositon() const noexcept;
        math::vec3f& GetPositon() noexcept;

        bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept override;

    protected:
        math::vec3f m_position;
    };
}
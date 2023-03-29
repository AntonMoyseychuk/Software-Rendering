#pragma once
#include "light.hpp"

namespace gfx {
    class PointLigth : public ILight {
    public:
        PointLigth() = default;
        PointLigth(const math::vec3f& position, Color color, float intensity);

        bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept override;

        void MoveFor(const math::vec3f& dist) noexcept;
        
    #pragma region getters-setters
        void SetPositon(const math::vec3f& position) noexcept;
        const math::vec3f& GetPositon() const noexcept;
    #pragma endregion getters-setters

    protected:
        math::vec3f m_position;
    };
}
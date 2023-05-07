#pragma once
#include "light.hpp"
#include "math_3d/matrix_operations.hpp"

namespace raytracing::gfx {
    class DirectionalLigth : public ILight {
    public:
        DirectionalLigth() = default;
        DirectionalLigth(const math::vec3f& direction, Color color, float intensity);

        bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept override;

        void Rotate(const math::mat4f& rotation_mat) noexcept;

    #pragma region getters-setters
        void SetDirection(const math::vec3f& direction) noexcept;
        const math::vec3f& GetDirection() const noexcept;
    #pragma endregion getters-setters

    private:
        math::vec3f m_direction;
    };
}
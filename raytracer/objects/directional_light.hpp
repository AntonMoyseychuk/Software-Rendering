#pragma once
#include "light.hpp"
#include "math_3d/matrix_operations.hpp"

namespace gfx {
    class DirectionalLigth : public ILight {
    public:
        DirectionalLigth() = default;
        DirectionalLigth(const math::vec3f& direction, const gfx::Color& color, float intensity);
        ~DirectionalLigth() override;

        bool ComputeIllumination(const math::vec3f& at_point, const math::vec3f& at_normal,
            gfx::Color& light_color, float& intensity) const noexcept override;

        void Rotate(const math::mat4f& rotation_mat) noexcept;

        void SetDirection(const math::vec3f& direction) noexcept;
        const math::vec3f& GetDirection() const noexcept;
        math::vec3f& GetDirection() noexcept;

    private:
        math::vec3f m_direction;
    };
}
#pragma once
#include "ray.hpp"
#include "math_3d/math.hpp"

#include <vector>

namespace raytracing::gfx {
    class Camera {
    public:
        Camera(const math::vec3& position, const math::vec3& look_at, const math::vec3& up, float fov_degrees, float aspect_ratio);

        const std::vector<gfx::Ray>& GenerateRays() const noexcept;

        void Rotate(float angle_radians, const math::vec2& axis) noexcept;
        void MoveFor(const math::vec3& offset) noexcept;

    #pragma region getters-setters
        void SetAspectRatio(float aspect) noexcept;
        float GetAspectRatio() const noexcept;

        void SetViewportSize(const math::vec2& size) const noexcept;

        const math::vec3& GetPosition() const noexcept;
        const math::vec3& GetForward() const noexcept;
        const math::vec3& GetRight() const noexcept;
        const math::vec3& GetUp() const noexcept;

        // math::mat4f GetView() const noexcept;
    #pragma endregion getters-setters

    private:
        void _RecalculateRays() const noexcept;

    private:
        math::vec3 m_position = math::vec3::ZERO;
        math::vec3 m_forward = math::vec3::BACKWARD;
        math::vec3 m_right = math::vec3::RIGHT;
        math::vec3 m_up = math::vec3::UP;
        const float m_radius = 1.0f;
        float m_thi_radians = math::MATH_PI;
        float m_theta_radians = math::MATH_PI_DIV_2;

        float m_tan_fov_div2 = 0.0f;
        float m_aspect_ratio = 0.0f;

        mutable std::vector<gfx::Ray> m_ray_cache;
        mutable math::vec2 m_ray_cache_size;
    };
}
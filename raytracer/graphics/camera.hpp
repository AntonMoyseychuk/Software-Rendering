#pragma once
#include "ray.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    class Camera {
    public:
        Camera(const math::vec3f& position, const math::vec3f& look_at, const math::vec3f& up, float fov_degrees, float aspect_ratio);

        const std::vector<gfx::Ray>& GenerateRays() const noexcept;

        void Rotate(float angle_radians, const math::vec2f& axis) noexcept;
        void MoveFor(const math::vec3f& offset) noexcept;

    #pragma region getters-setters
        void SetAspectRatio(float aspect) noexcept;
        float GetAspectRatio() const noexcept;

        void SetViewportSize(const math::vec2ui& size) const noexcept;

        const math::vec3f& GetPosition() const noexcept;
        const math::vec3f& GetForward() const noexcept;
        const math::vec3f& GetRight() const noexcept;
        const math::vec3f& GetUp() const noexcept;

        math::mat4f GetView() const noexcept;
    #pragma endregion getters-setters

    private:
        void _RecalculateRays() const noexcept;

    private:
        math::vec3f m_position;
        math::vec3f m_forward;
        math::vec3f m_right;
        math::vec3f m_up;

        float m_tan_fov_div2 = 0.0f;
        float m_aspect_ratio = 0.0f;

        mutable std::vector<gfx::Ray> m_ray_cache;
        mutable math::vec2ui m_ray_cache_size;
    };
}
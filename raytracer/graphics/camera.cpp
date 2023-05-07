#include "camera.hpp"

#include <iostream>

namespace raytracing::gfx {
    Camera::Camera(const math::vec3f &position, const math::vec3f &look_at, const math::vec3f &up, float fov_degrees, float aspect_ratio)
        : m_position(position),
            m_forward(math::Normalize(look_at - position)),
            m_right(math::Normalize(math::Cross(up, -m_forward))),
            m_up(math::Cross(-m_forward, m_right)),
            m_radius(1.0f),
            m_thi_radians(math::Angle(math::Normalize(math::vec3f(m_forward.x, 0.0f, m_forward.z)), math::VECTOR_FORWARD)),
            m_theta_radians(math::Angle(math::Normalize(math::vec3f(m_forward.x, m_forward.y, 0.0f)), math::VECTOR_UP)),
            m_tan_fov_div2(tanf(math::ToRadians(fov_degrees) / 2.0f)), 
            m_aspect_ratio(aspect_ratio)
    {
    }

    const std::vector<gfx::Ray>& Camera::GenerateRays() const noexcept {
        return m_ray_cache;
    }
    
    void Camera::Rotate(float angle_radians, const math::vec2f& axis) noexcept {
        if (!math::IsTendsTo(angle_radians, 0.0f)) {
            using namespace math;

            m_thi_radians += axis.y * angle_radians;
            m_theta_radians += axis.x * angle_radians;

            const auto forward_x = m_radius * std::sinf(m_theta_radians) * std::sinf(m_thi_radians);
            const auto forward_y = m_radius * std::cosf(m_theta_radians);
            const auto forward_z = m_radius * std::sinf(m_theta_radians) * std::cosf(m_thi_radians);
            m_forward = Normalize(vec3f(
                IsTendsTo(forward_x, 0.0f) ? 0.0f : forward_x,
                IsTendsTo(forward_y, 0.0f) ? 0.0f : forward_y,
                IsTendsTo(forward_z, 0.0f) ? 0.0f : forward_z
            ));
            m_right = Cross(VECTOR_UP, -m_forward);
            m_up = Cross(-m_forward, m_right);

            this->_RecalculateRays();
        }
    }

    void Camera::MoveFor(const math::vec3f& offset) noexcept {
        if (offset != math::vec3f(0.0f)) {
            m_position += offset;
            
            this->_RecalculateRays();
        }
    }

    void Camera::SetAspectRatio(float aspect) noexcept {
        if (aspect > 0.0f) {
            m_aspect_ratio = aspect;
            
            this->_RecalculateRays();
        }
    }

    float Camera::GetAspectRatio() const noexcept {
        return m_aspect_ratio;
    }

    void Camera::SetViewportSize(const math::vec2ui &new_size) const noexcept {
        if (m_ray_cache_size != new_size) {
            m_ray_cache_size = new_size;
            m_ray_cache.resize(new_size.x * new_size.y);
            
            this->_RecalculateRays();
        }
    }

    const math::vec3f &Camera::GetPosition() const noexcept {
        return m_position;
    }

    const math::vec3f &Camera::GetForward() const noexcept {
        return m_forward;
    }

    const math::vec3f &Camera::GetRight() const noexcept {
        return m_right;
    }

    const math::vec3f &Camera::GetUp() const noexcept {
        return m_up;
    }

    // math::mat4f Camera::GetView() const noexcept {
    //     return math::LookAtRH(m_position, m_position + m_forward, m_up);
    // }

    void Camera::_RecalculateRays() const noexcept {
        const auto dx = 2.0f / m_ray_cache_size.x;
        const auto dy = 2.0f / m_ray_cache_size.y;

        for (std::size_t y = 0; y < m_ray_cache_size.y; ++y) {
            for (std::size_t x = 0; x < m_ray_cache_size.x; ++x) {
                const float pixel_x = m_forward.x + (-1.0f + (x * dx)) * m_aspect_ratio * m_tan_fov_div2;
                const float pixel_y = m_forward.y + (1.0f - (y * dy)) * m_tan_fov_div2;

                const auto ray_dir = math::Normalize(m_forward + m_right * pixel_x + m_up * pixel_y);

                m_ray_cache[x + y * m_ray_cache_size.x] = Ray(m_position, ray_dir);
            }
        }
    }
}
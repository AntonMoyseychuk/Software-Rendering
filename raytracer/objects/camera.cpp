#include "camera.hpp"

namespace gfx {
    Camera::Camera(const math::vec3f &position, const math::vec3f &look_at, const math::vec3f &up, float fov_degrees, float aspect_ratio)
        : m_view(math::LookAtRH(position, look_at, up)),
            m_position(position),
            m_forward(math::Normalize(look_at - position)),
            m_right(math::Normalize(math::Cross(up, -m_forward))),
            m_up(math::Cross(-m_forward, m_right)),
            m_tan_fov_div2(tanf(math::ToRadians(fov_degrees) / 2.0f)), 
            m_aspect_ratio(aspect_ratio)
    {

    }

    const std::vector<gfx::Ray>& Camera::GenerateRays() const noexcept {
        return m_ray_cache;
    }
    
    void Camera::Rotate(float angle_radians, const math::vec2f &axis) noexcept {
        m_view = math::RotateX(m_view, angle_radians * axis.x);
        m_view = math::RotateY(m_view, angle_radians * axis.y);

        m_forward = m_forward * m_view;

        this->_RecalculateRays();
    }

    float Camera::GetFOVInDegrees() const noexcept {
        return math::ToDegrees(atanf(m_tan_fov_div2) * 2.0f);
    }

    float Camera::GetFOVInRadians() const noexcept {
        return atanf(m_tan_fov_div2) * 2.0f;
    }

    void Camera::SetAspectRatio(float aspect) noexcept {
        if (aspect > 0.0f) {
            m_aspect_ratio = aspect;
        }

        this->_RecalculateRays();
    }

    float Camera::GetAspectRatio() const noexcept {
        return m_aspect_ratio;
    }

    void Camera::SetViewportSize(const math::vec2ui &new_size) const noexcept {
        if (m_ray_cache_size != new_size) {
            m_ray_cache_size = new_size;
            m_ray_cache.resize(new_size.x * new_size.y);
        }

        this->_RecalculateRays();
    }

    const math::vec2ui &Camera::GetRayCacheSize() const noexcept {
        return m_ray_cache_size;
    }

    const math::vec3f &Camera::GetPosition() const noexcept {
        return m_position;
    }

    void Camera::_RecalculateRays() const noexcept {
        const auto dx = 2.0f / m_ray_cache_size.x;
        const auto dy = 2.0f / m_ray_cache_size.y;

        for (std::size_t y = 0; y < m_ray_cache_size.y; ++y) {
            for (std::size_t x = 0; x < m_ray_cache_size.x; ++x) {
                // float pixel_x = m_forward.x + (-1.0f + (x * dx)) * m_aspect_ratio * m_tan_fov_div2;
                // float pixel_y = m_forward.y + (1.0f - (y * dy)) * m_tan_fov_div2;

                const auto ray_dir = m_forward + math::vec3f(
                    (-1.0f + (x * dx)) * m_aspect_ratio * m_tan_fov_div2, 
                    (1.0f - (y * dy)) * m_tan_fov_div2, 
                    m_forward.z
                );

                m_ray_cache[x + y * m_ray_cache_size.x] = Ray(m_position, math::Normalize(ray_dir));
            }
        }
    }
}
#include "camera.hpp"
#include "math_3d/math.hpp"


namespace raytracing::gfx {
    Camera::Camera(const math::vec3 &position, const math::vec3 &look_at, const math::vec3 &up, float fov_degrees, float aspect_ratio)
        : m_position(position),
            m_forward(math::normalize(look_at - position)),
            m_right(math::normalize(math::cross(up, -m_forward))),
            m_up(math::cross(-m_forward, m_right)),
            m_radius(1.0f),
            m_thi_radians(math::angle(math::normalize(math::vec3(m_forward.x, 0.0f, m_forward.z)), math::vec3::FORWARD)),
            m_theta_radians(math::angle(m_forward, math::vec3::UP)),
            m_tan_fov_div2(tanf(math::to_radians(fov_degrees) / 2.0f)), 
            m_aspect_ratio(aspect_ratio)
    {
    }

    const std::vector<gfx::Ray>& Camera::GenerateRays() const noexcept {
        return m_ray_cache;
    }
    
    void Camera::Rotate(float angle_radians, const math::vec2& axis) noexcept {
        if (!math::is_tends_to(angle_radians, 0.0f)) {
            using namespace math;

            m_thi_radians += axis.y * angle_radians;
            m_theta_radians += axis.x * angle_radians;

            const auto forward_x = m_radius * std::sinf(m_theta_radians) * std::sinf(m_thi_radians);
            const auto forward_y = m_radius * std::cosf(m_theta_radians);
            const auto forward_z = m_radius * std::sinf(m_theta_radians) * std::cosf(m_thi_radians);
            m_forward = normalize(vec3(
                is_tends_to(forward_x, 0.0f) ? 0.0f : forward_x,
                is_tends_to(forward_y, 0.0f) ? 0.0f : forward_y,
                is_tends_to(forward_z, 0.0f) ? 0.0f : forward_z
            ));
            m_right = cross(vec3::UP, -m_forward);
            m_up = cross(-m_forward, m_right);

            this->_RecalculateRays();
        }
    }

    void Camera::MoveFor(const math::vec3& offset) noexcept {
        if (offset != math::vec3(0.0f)) {
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

    void Camera::SetViewportSize(const math::vec2& new_size) const noexcept {
        if (m_ray_cache_size != new_size) {
            m_ray_cache_size = new_size;
            m_ray_cache.resize(uint32_t(new_size.x) * uint32_t(new_size.y));
            
            this->_RecalculateRays();
        }
    }

    const math::vec3 &Camera::GetPosition() const noexcept {
        return m_position;
    }

    const math::vec3 &Camera::GetForward() const noexcept {
        return m_forward;
    }

    const math::vec3 &Camera::GetRight() const noexcept {
        return m_right;
    }

    const math::vec3 &Camera::GetUp() const noexcept {
        return m_up;
    }

    // math::mat4f Camera::GetView() const noexcept {
    //     return math::LookAtRH(m_position, m_position + m_forward, m_up);
    // }

    void Camera::_RecalculateRays() const noexcept {
        const float dx = 2.0f / m_ray_cache_size.x;
        const float dy = 2.0f / m_ray_cache_size.y;

        const size_t width = m_ray_cache_size.x, height = m_ray_cache_size.y;

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                const float pixel_x = m_forward.x + (-1.0f + (x * dx)) * m_aspect_ratio * m_tan_fov_div2;
                const float pixel_y = m_forward.y + (1.0f - (y * dy)) * m_tan_fov_div2;

                const auto ray_dir = math::normalize(m_forward + m_right * pixel_x + m_up * pixel_y);

                m_ray_cache[x + y * width] = Ray(m_position, ray_dir);
            }
        }
    }
}
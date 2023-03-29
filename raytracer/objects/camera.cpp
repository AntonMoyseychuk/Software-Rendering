#include "camera.hpp"

namespace gfx {
    Camera::Camera(const math::vec3f &position, const math::vec3f &look_at, const math::vec3f &up, float fov_degrees, float aspect_ratio)
        : m_tan_fov_div2(tanf(math::ToRadians(fov_degrees) / 2.0f)), m_aspect_ratio(aspect_ratio), m_forward(math::Normalize(look_at - position)), m_position(position)
    {

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
    }

    float Camera::GetAspectRatio() const noexcept {
        return m_aspect_ratio;
    }

    const math::vec2ui &Camera::GetRayCacheSize() const noexcept {
        return m_ray_cache_size;
    }

    const math::vec3f &Camera::GetPosition() const noexcept {
        return m_position;
    }

    const std::vector<gfx::Ray>& Camera::GenerateRays(const math::vec2ui &screen_size) const noexcept {
        if (screen_size.x != m_ray_cache_size.x || screen_size.y != m_ray_cache_size.y) {
            m_ray_cache.resize(screen_size.x * screen_size.y);
            m_ray_cache_size = screen_size;

            const auto dx = 2.0f / screen_size.x;
            const auto dy = 2.0f / screen_size.y;

            for (std::size_t y = 0; y < screen_size.y; ++y) {
                for (std::size_t x = 0; x < screen_size.x; ++x) {
                    float pixel_x = m_forward.x + (-1.0f + (x * dx)) * m_aspect_ratio * m_tan_fov_div2;
                    float pixel_y = m_forward.y + (1.0f - (y * dy)) * m_tan_fov_div2;

                    m_ray_cache[x + y * screen_size.x] = Ray(m_position, math::Normalize(math::vec3f(pixel_x, pixel_y, m_forward.z)));
                }
            }
        }

        return m_ray_cache;
    }
}
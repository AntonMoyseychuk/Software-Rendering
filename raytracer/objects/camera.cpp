#include "camera.hpp"

namespace gfx {
    Camera::Camera(const math::vec3f &position, const math::vec3f &look_at, const math::vec3f &up, float fov_degrees, float aspect_ratio)
        : IObject(position), m_fov_degrees(fov_degrees), m_aspect_ratio(aspect_ratio), m_dir(math::Normalize(look_at - position))
    {

    }

    float Camera::GetFOVInDegrees() const noexcept {
        return m_fov_degrees;
    }

    float Camera::GetFOVInRadians() const noexcept {
        return math::ToRadians(m_fov_degrees);
    }

    void Camera::SetAspectRatio(float aspect) noexcept {
        if (aspect > 0.0f) {
            m_aspect_ratio = aspect;
        }
    }

    float Camera::GetAspectRatio() const noexcept
    {
        return m_aspect_ratio;
    }

    const math::vec2ui &Camera::GetRayCacheSize() const noexcept {
        return m_ray_cache_size;
    }

    const std::vector<gfx::Ray>& Camera::GenerateRays(const math::vec2ui &screen_size) const noexcept {
        if (screen_size.x != m_ray_cache_size.x || screen_size.y != m_ray_cache_size.y) {
            m_ray_cache.resize(screen_size.x * screen_size.y);
            m_ray_cache_size = screen_size;

            const float fov = tanf(math::ToRadians(m_fov_degrees) / 2.0f);
            const auto dx = 2.0f / screen_size.x;
            const auto dy = 2.0f / screen_size.y;

            for (std::size_t y = 0; y < screen_size.y; ++y) {
                for (std::size_t x = 0; x < screen_size.x; ++x) {
                    float pixel_x = (-1.0f + (x * dx)) * m_aspect_ratio * fov;
                    float pixel_y = (1.0f - (y * dy)) * fov;

                    m_ray_cache[x + y * screen_size.x] = Ray(m_position, math::Normalize(math::vec3f(pixel_x, pixel_y, m_dir.z)));
                }
            }
        }

        return m_ray_cache;
    }
}
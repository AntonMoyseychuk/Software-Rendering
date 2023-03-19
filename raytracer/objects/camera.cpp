#include "camera.hpp"

namespace gfx {
    Camera::Camera(const math::vec3f& position, std::uint32_t viewport_width, std::uint32_t viewport_height, float fov_in_degrees)
        : IObject(position), m_fov_degrees(fov_in_degrees), m_viewport_width(viewport_width), m_viewport_height(viewport_height)
    {
    }

    float Camera::GetFOVInDegrees() const noexcept {
        return m_fov_degrees;
    }

    float Camera::GetFOVInRadians() const noexcept {
        return math::ToRadians(m_fov_degrees);
    }

    std::uint32_t Camera::GetViewportWidth() const noexcept {
        return m_viewport_width;
    }

    void Camera::SetViewportWidth(std::uint32_t width) noexcept {
        m_viewport_width = width;
    }

    std::uint32_t Camera::GetViewportHeight() const noexcept {
        return m_viewport_height;
    }

    void Camera::SetViewportHeight(std::uint32_t height) noexcept {
        m_viewport_height = height;
    }
    
    float Camera::GetAspectRatio() const noexcept {
        return static_cast<float>(m_viewport_width) / static_cast<float>(m_viewport_height);
    }
}
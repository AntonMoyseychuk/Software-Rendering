#pragma once
#include "object.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    class Camera : public IObject {
    public:
        Camera(const math::vec3f& position, std::uint32_t viewport_width, std::uint32_t viewport_height, float fov_in_degrees);

        float GetFOVInDegrees() const noexcept;
        float GetFOVInRadians() const noexcept;

        std::uint32_t GetViewportWidth() const noexcept;
        void SetViewportWidth(std::uint32_t width) noexcept;

        std::uint32_t GetViewportHeight() const noexcept;
        void SetViewportHeight(std::uint32_t height) noexcept;

        float GetAspectRatio() const noexcept;

    private:
        math::mat4f m_view;

        float m_fov_degrees = 45.0f;
        std::uint32_t m_viewport_width = 0;
        std::uint32_t m_viewport_height = 0;
    };
}
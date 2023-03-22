#pragma once
#include "graphics/ray.hpp"
#include "graphics/color.hpp"

#include <memory>
#include <list>

namespace gfx {
    class ILight {
    public:
        ILight() = default;
        ILight(const gfx::Color& color, float intensity) : m_color(color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}
        virtual ~ILight() {}

        virtual bool ComputeIllumination(const math::vec3f& at_point, const math::vec3f& at_normal, 
            gfx::Color& light_color, float& intensity) const noexcept = 0;

        void SetColor(const Color& color) noexcept { m_color = color; }
        const Color& GetColor() const noexcept { return m_color; }
        Color& GetColor() noexcept { return m_color; }

        void SetIntensity(float intensity) noexcept { m_intensity = intensity; }
        float GetIntensity() const noexcept { return m_intensity; }
        float& GetIntensity() noexcept { return m_intensity; }

    protected:
        gfx::Color m_color;
        float m_intensity;
    };
}
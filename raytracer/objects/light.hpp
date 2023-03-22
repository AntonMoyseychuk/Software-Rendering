#pragma once
#include "graphics/ray.hpp"
#include "graphics/color.hpp"

#include "graphics/intersection_data.hpp"

#include <memory>
#include <list>

namespace gfx {
    class ILight {
    public:
        ILight() = default;
        ILight(const gfx::Color& color, float intensity) : m_color(color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}
        virtual ~ILight() {}

        virtual bool ComputeIllumination(const IntersectionData& int_data,
            gfx::Color& out_light_color, float& out_intensity) const noexcept = 0;

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
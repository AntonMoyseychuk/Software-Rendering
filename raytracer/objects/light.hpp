#pragma once
#include "graphics/color.hpp"
#include "objects/drawable.hpp"
#include "graphics/intersection_data.hpp"

#include <memory>
#include <list>

namespace gfx {
    class ILight {
    public:
        ILight() = default;
        ILight(Color color, float intensity) : m_color(color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}
        virtual ~ILight() {}

        virtual bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept = 0;

    #pragma region getters-setters
        void SetColor(Color color) noexcept { m_color = color; }
        Color GetColor() const noexcept { return m_color; }

        void SetIntensity(float intensity) noexcept { m_intensity = intensity; }
        float GetIntensity() const noexcept { return m_intensity; }
    #pragma endregion getters-setters

    protected:
        Color m_color;
        float m_intensity;
    };
}
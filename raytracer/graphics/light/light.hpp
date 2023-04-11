#pragma once
#include "graphics/color.hpp"
#include "graphics/drawables/drawable.hpp"
#include "graphics/intersection_data.hpp"

#include <memory>
#include <list>

namespace gfx {
    struct ILight {
        ILight() = default;
        ILight(Color _color, float intensity) : color(_color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}
        virtual ~ILight() {}

        virtual bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept = 0;

    #pragma region getters-setters
        void SetIntensity(float intensity) noexcept { m_intensity = intensity < 0.0f ? 0.0f : intensity; }
        float GetIntensity() const noexcept { return m_intensity; }
        float& GetIntensity() noexcept { return m_intensity; }
    #pragma endregion getters-setters

    protected:
        float m_intensity;
    
    public:
        Color color;
    };
}
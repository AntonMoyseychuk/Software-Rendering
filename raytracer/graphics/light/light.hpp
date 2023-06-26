#pragma once
#include "math_3d/vec4.hpp"
#include "graphics/drawables/drawable.hpp"
#include "graphics/intersection_data.hpp"

#include <memory>
#include <list>

namespace raytracing::gfx {
    struct ILight {
        ILight() = default;
        ILight(const math::color& _color, float intensity) : color(_color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}
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
        math::color color;
    };
}
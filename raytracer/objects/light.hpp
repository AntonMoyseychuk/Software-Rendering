#pragma once
#include "object.hpp"
#include "drawable.hpp"
#include "graphics/ray.hpp"
#include "graphics/color.hpp"

#include <memory>
#include <list>

namespace gfx {
    class ILight : public IObject {
    public:
        virtual ~ILight() = 0 {}
        
        ILight() = default;
        ILight(const math::vec3f& position, const gfx::Color& color, float intensity) 
            : IObject(position), m_color(color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}

        virtual bool ComputeIllumination(const math::vec3f& at_point, const math::vec3f& at_normal, 
            const std::list<std::shared_ptr<IDrawable>>& drawables, 
            const std::shared_ptr<IObject>& curr_obj, 
            gfx::Color& color, float& intensity) const noexcept = 0;

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
#pragma once
#include "object.hpp"
#include "drawable.hpp"
#include "graphics/ray.hpp"
#include "graphics/material.hpp"

#include <memory>
#include <list>

namespace gfx {
    class ILight : public IObject {
    public:
        virtual ~ILight() = 0 {}
        
        ILight() = default;
        ILight(const gfx::Color& color, float intensity) 
            : m_color(color), m_intensity(intensity < 0.0f ? 0.0f : intensity) {}

        virtual bool ComputeIllumination(const math::vec4f& intersect_point, const math::vec4f& normal, 
            const std::list<std::shared_ptr<IDrawable>>& drawables, const std::shared_ptr<IObject>& curr_obj, gfx::Material& material) = 0;

    private:
        gfx::Color m_color;
        float m_intensity;
    };
}
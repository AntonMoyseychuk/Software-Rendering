#pragma once
#include "point_light.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    PointLigth::PointLigth(const math::vec3f& position, const gfx::Color &color, float intensity)
        : ILight(position, color, intensity)
    {
    }
    
    PointLigth::~PointLigth()
    {
    }
    
    bool PointLigth::ComputeIllumination(const math::vec3f& intersect_point, const math::vec3f& normal, 
            const std::list<std::shared_ptr<IDrawable>>& drawables, 
            const std::shared_ptr<IObject>& curr_obj, 
            gfx::Color& color, float& intensity) const noexcept
    {
        auto light_dir = (m_position - intersect_point).Normalize();
        auto angle = acosf(math::Dot(normal, light_dir));

        color = m_color;
        if (angle > math::MATH_PI / 2) {
            intensity = 0;
            return false;
        }

        intensity = m_intensity * (1.0f - (angle / (math::MATH_PI / 2)));
        return true;
    }
}
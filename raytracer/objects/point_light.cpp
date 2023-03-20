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
    
    bool PointLigth::ComputeIllumination(
        const math::vec3f& at_point, const math::vec3f& at_normal, 
        const std::list<std::shared_ptr<IDrawable>>& drawables, const std::shared_ptr<IObject>& curr_obj, 
        gfx::Color& color, float& intensity) const noexcept
    {
        auto light_dir = math::Normalize(m_position - at_point);
        auto angle = acosf(math::Dot(at_normal, light_dir));

        color = m_color;
        if (angle > math::MATH_PI / 2) {
            intensity = 0.0f;
            return false;
        }

        intensity = intensity * (1.0f - (angle / (math::MATH_PI / 2)));
        return true;
    }
}
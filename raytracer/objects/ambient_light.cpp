#include "ambient_light.hpp"

namespace gfx {
    AmbientLight::AmbientLight(const gfx::Color &color, float intensity)
        : ILight(color, intensity)
    {
    }
    
    bool AmbientLight::ComputeIllumination(const IntersectionData& int_data, const math::vec3f& camera_pos, 
        gfx::Color& light_color, float& intensity) const noexcept 
    {
        light_color = m_color;
        intensity += m_intensity;
        return true;
    }
}
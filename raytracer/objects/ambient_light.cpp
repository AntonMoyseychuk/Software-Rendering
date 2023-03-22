#include "ambient_light.hpp"

namespace gfx {
    AmbientLight::AmbientLight(const gfx::Color &color, float intensity)
        : ILight(color, intensity)
    {
    }
    
    bool AmbientLight::ComputeIllumination(const math::vec3f &at_point, const math::vec3f &at_normal, 
        gfx::Color &color, float &intensity) const noexcept
    {
        color = m_color;
        intensity += m_intensity;
        return true;
    }
}
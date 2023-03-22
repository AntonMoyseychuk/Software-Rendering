#include "ambient_light.hpp"

namespace gfx {
    AmbientLight::AmbientLight(const gfx::Color &color, float intensity)
        : ILight(color, intensity)
    {
    }
    
    bool AmbientLight::ComputeIllumination(const IntersectionData& int_data,
        gfx::Color& out_light_color, float& out_intensity) const noexcept 
    {
        out_light_color = m_color;
        out_intensity += m_intensity;
        return true;
    }
}
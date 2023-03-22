#include "ambient_light.hpp"

namespace gfx {
    AmbientLight::AmbientLight(const gfx::Color &color, float intensity)
        : ILight(color, intensity)
    {
    }
    
    bool AmbientLight::ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept 
    {
        out_intensity += m_intensity;
        return true;
    }
}
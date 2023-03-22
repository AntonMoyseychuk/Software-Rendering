#include "directional_light.hpp"

namespace gfx {
    DirectionalLigth::DirectionalLigth(const math::vec3f &direction, const gfx::Color &color, float intensity)
        : ILight(color, intensity), m_direction(math::Normalize(direction)) 
    {
    }
    
    bool DirectionalLigth::ComputeIllumination(const math::vec3f &at_point, const math::vec3f &at_normal, 
        gfx::Color &light_color, float &intensity) const noexcept 
    {
        #if 0
            const auto angle = acosf(math::Dot(math::Normalize(at_normal), -m_direction)); // angle = (0, PI)
                
            if (angle > math::MATH_PI_DIV_2) {
                // intensity = 0.0f;
                return false;
            } // angle = (0, PI_DIV_2)

            light_color = m_color;
            intensity += m_intensity * (1.0f - (angle / math::MATH_PI_DIV_2));
            return true;
        #else
            const auto cos_angle = math::Dot(math::Normalize(at_normal), -m_direction); // angle = (0, PI)
                
            if (cos_angle < 0.0f) {
                // intensity = 0.0f;
                return false;
            } // angle = (0, PI_DIV_2)

            light_color = m_color;
            intensity += m_intensity * cos_angle;
            return true;
        #endif
    }

    void DirectionalLigth::Rotate(const math::mat4f &rotation_mat) noexcept {
        m_direction = math::vec3f(m_direction * rotation_mat);
    }

    void DirectionalLigth::SetDirection(const math::vec3f &direction) noexcept {
        m_direction = math::Normalize(direction);
    }
    
    const math::vec3f &DirectionalLigth::GetDirection() const noexcept {
        return m_direction;
    }

    math::vec3f &DirectionalLigth::GetDirection() noexcept {
        return m_direction;
    }
}
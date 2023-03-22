#include "directional_light.hpp"

namespace gfx {
    DirectionalLigth::DirectionalLigth(const math::vec3f &direction, const gfx::Color &color, float intensity)
        : ILight(color, intensity), m_direction(math::Normalize(direction)) 
    {
    }
    
    bool DirectionalLigth::ComputeIllumination(const IntersectionData& int_data, const math::vec3f& camera_pos, 
        gfx::Color& light_color, float& intensity) const noexcept 
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
            const auto cos_angle = math::Dot(math::Normalize(int_data.normal), -m_direction); // angle = (0, PI)
                
            if (cos_angle < 0.0f) {
                // intensity = 0.0f;
                return false;
            } // angle = (0, PI_DIV_2)

            light_color = m_color;
            intensity += m_intensity * cos_angle;

            if (int_data.material.specular_index > 0) {
                const auto R = math::Normalize(math::Reflect(int_data.point, int_data.normal));
                const auto V = -math::Normalize((int_data.point - camera_pos));
                const auto r_dot_v = math::Dot(R, V);
                if (r_dot_v > 0) {
                    intensity += m_intensity * powf(r_dot_v, int_data.material.specular_index);
                }
            }
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
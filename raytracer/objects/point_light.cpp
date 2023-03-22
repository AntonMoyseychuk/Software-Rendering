#pragma once
#include "point_light.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    PointLigth::PointLigth(const math::vec3f& position, const gfx::Color &color, float intensity)
        : ILight(color, intensity), m_position(position)
    {
    }

    void PointLigth::MoveFor(const math::vec3f &dist) noexcept {
        m_position += dist;
    }

    void PointLigth::SetPositon(const math::vec3f &position) noexcept {
        m_position = position;
    }

    const math::vec3f &PointLigth::GetPositon() const noexcept {
        return m_position;
    }

    math::vec3f &PointLigth::GetPositon() noexcept {
        return m_position;
    }

    bool PointLigth::ComputeIllumination(const IntersectionData& int_data, const math::vec3f& camera_pos, 
        gfx::Color& light_color, float& intensity) const noexcept 
    {
        #if 0
            const auto light_dir = math::Normalize(m_position - at_point);
            const auto angle = acosf(math::Dot(at_normal, light_dir));


            if (angle > math::MATH_PI_DIV_2) {
                // intensity = 0.0f;
                return false;
            }

            light_color = m_color;
            intensity += m_intensity * (1.0f - (angle / math::MATH_PI_DIV_2));
            return true;
        #else
            const auto light_dir = -math::Normalize(int_data.point - m_position);
            const auto cos_angle = math::Dot(int_data.normal, light_dir);


            if (cos_angle < 0.0f) {
                // intensity = 0.0f;
                return false;
            }

            light_color = m_color;
            intensity += m_intensity * (cos_angle);

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
}
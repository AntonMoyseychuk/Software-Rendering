#pragma once
#include "point_light.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    PointLigth::PointLigth(const math::vec3f& position, Color color, float intensity)
        : ILight(color, intensity), m_position(position)
    {
    }

    bool PointLigth::ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
        float& out_intensity) const noexcept 
    {
        const auto light_dir = -math::Normalize(int_data.point - m_position);
        const auto cos_angle = math::Dot(int_data.normal, light_dir);

        if (cos_angle < 0.0f) {
            return false;
        }

        const auto ray_to_light = gfx::Ray(int_data.point + math::vec3f(math::MATH_EPSILON), math::Normalize(m_position - int_data.point));
        for (const auto& drawable : drawables) {
            if (drawable->IsIntersect(ray_to_light)) {
                return false;
            }
        }

        out_intensity += m_intensity * (cos_angle);

        if (int_data.material.specular_index > 0) {
            const auto R = math::Normalize(math::Reflect(int_data.point, int_data.normal));
            const auto V = -math::Normalize((int_data.point - int_data.casted_ray.origin));
            const auto r_dot_v = math::Dot(R, V);
            if (r_dot_v > 0) {
                out_intensity += m_intensity * powf(r_dot_v, int_data.material.specular_index);
            }
        }

        return true;
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
}
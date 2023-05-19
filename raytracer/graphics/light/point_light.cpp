#pragma once
#include "point_light.hpp"
#include "math_3d/math.hpp"

#include "graphics/materials/glaring_material.hpp"

namespace raytracing::gfx {
    PointLigth::PointLigth(const math::vec3& _position, const math::color& _color, float _intensity)
        : ILight(_color, _intensity), position(_position)
    {
    }

    bool PointLigth::ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
        float& out_intensity) const noexcept 
    {
        using namespace math;

        const auto light_dir = -normalize(int_data.point - position);
        const auto cos_angle = dot(int_data.normal, light_dir);

        if (cos_angle < 0.0f) {
            return false;
        }

        const auto ray_to_light = gfx::Ray(int_data.point + vec3(math::MATH_EPSILON), normalize(position - int_data.point));
        for (const auto& drawable : drawables) {
            if (drawable->IsIntersect(ray_to_light)) {
                return false;
            }
        }

        out_intensity += m_intensity * (cos_angle);

        const IGlaringMaterial* glaring = dynamic_cast<IGlaringMaterial*>(int_data.material.get());
        if (glaring && glaring->specular_index > 0.0f) {
            const auto reflected_vec = reflect(int_data.casted_ray.direction, int_data.normal);
            const auto r_dot_v = dot(reflected_vec, -int_data.casted_ray.direction);
            if (r_dot_v > 0.0f) {
                out_intensity += m_intensity * std::powf(r_dot_v, glaring->specular_index);
            }
        }

        return true;
    }

    void PointLigth::MoveFor(const math::vec3 &dist) noexcept {
        position += dist;
    }
}
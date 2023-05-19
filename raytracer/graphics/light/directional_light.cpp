#include "directional_light.hpp"
#include "math_3d/vec_operations.hpp"
#include "math_3d/const.hpp"

#include "graphics/materials/glaring_material.hpp"

namespace raytracing::gfx {
    DirectionalLigth::DirectionalLigth(const math::vec3 &direction, const math::color& color, float intensity)
        : ILight(color, intensity), m_direction(math::normalize(direction)) 
    {
    }
    
    bool DirectionalLigth::ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
        float& out_intensity) const noexcept 
    {
        using namespace math;

        const auto cos_angle = dot(normalize(int_data.normal), -m_direction); // angle = (0, PI)
                
        if (cos_angle < 0.0f) {
            return false;
        } // angle = (0, PI_DIV_2)

        const auto ray_to_light = gfx::Ray(int_data.point + vec3(MATH_EPSILON), -m_direction);
        for (const auto& drawable : drawables) {
            if (drawable->IsIntersect(ray_to_light)) {
                return false;
            }
        }

        out_intensity += m_intensity * cos_angle;

        const IGlaringMaterial* glaring = dynamic_cast<IGlaringMaterial*>(int_data.material.get());
        if (glaring && glaring->specular_index > 0.0f) {
            const vec3 reflected_vec = reflect(int_data.casted_ray.direction, int_data.normal);
            const float r_dot_v = dot(reflected_vec, -int_data.casted_ray.direction);
            if (r_dot_v > 0.0f) {
                out_intensity += m_intensity * powf(r_dot_v, glaring->specular_index);
            }
        }
        return true;
    }

    void DirectionalLigth::Rotate(const math::mat4 &rotation_mat) noexcept {
        m_direction = math::vec3(m_direction * rotation_mat);
    }

    void DirectionalLigth::SetDirection(const math::vec3 &direction) noexcept {
        m_direction = math::normalize(direction);
    }
    
    const math::vec3& DirectionalLigth::GetDirection() const noexcept {
        return m_direction;
    }
}
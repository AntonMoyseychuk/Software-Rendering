#include "metal_material.hpp"

#include "graphics/intersection_data.hpp"
#include "math_3d/vec_operations.hpp"

namespace raytracing::gfx {
    Metal::Metal(const math::color& _color, float _reflection_index, float _specular_index)
        : IColoredMaterial(_color), IGlaringMaterial(_specular_index), reflection_index(_reflection_index < 1.0f ? _reflection_index : 1.0f)
    {
    }
    
    bool Metal::Scatter(const IntersectionData &int_data, math::color &attenuation, Ray &scattered_ray) const noexcept {
        using namespace math;

        const auto reflected = reflect(int_data.casted_ray.direction, int_data.normal);
        scattered_ray = Ray(int_data.point, reflected + reflection_index * vec3(random_unit_vector_in_hemisphere(vec4(int_data.normal))));
        
        attenuation = color;

        return dot(reflected, int_data.normal) > 0.0f;
    }
}
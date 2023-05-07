#include "metal_material.hpp"

#include "graphics/intersection_data.hpp"
#include "math_3d/vector_operations.hpp"

namespace raytracing::gfx {
    Metal::Metal(Color _color, float _reflection_index, float _specular_index)
        : IColoredMaterial(_color), IGlaringMaterial(_specular_index), reflection_index(_reflection_index < 1.0f ? _reflection_index : 1.0f)
    {
    }
    
    bool Metal::Scatter(const IntersectionData &int_data, Color &attenuation, Ray &scattered_ray) const noexcept {
        const auto reflected = math::Reflect(int_data.casted_ray.direction, int_data.normal);
        scattered_ray = Ray(int_data.point, reflected + reflection_index * math::RandomUnitVectorInHemisphere(int_data.normal));
        
        attenuation = color;

        return math::Dot(reflected, int_data.normal) > 0.0f;
    }
}
#include "metal_material.hpp"

#include "graphics/intersection_data.hpp"
#include "math_3d/vector_operations.hpp"

namespace gfx {
    Metal::Metal(Color color, float reflection_index)
        : IColoredMaterial(color), m_reflection_index(reflection_index < 1.0f ? reflection_index : 1.0f)
    {
    }
    
    bool Metal::Scatter(const IntersectionData &int_data, Color &attenuation, Ray &scattered_ray) const noexcept {
        const auto reflected = math::Reflect(int_data.casted_ray.direction, int_data.normal);
        scattered_ray = Ray(int_data.point, reflected + m_reflection_index * math::RandomUnitVectorInHemisphere(int_data.normal));
        
        attenuation = m_color;

        return math::Dot(reflected, int_data.normal) > 0.0f;
    }
    
    void Metal::SetReflectionIndex(float reflection_index) noexcept {
        m_reflection_index = reflection_index;
    }
    
    float Metal::GetReflectionIndex() noexcept {
        return m_reflection_index;
    }
}
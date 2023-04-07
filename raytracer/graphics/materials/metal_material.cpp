#include "metal_material.hpp"

#include "graphics/intersection_data.hpp"
#include "math_3d/vector_operations.hpp"

namespace gfx {
    Metal::Metal(Color color, float fuzz_index)
        : IColoredMaterial(color), m_fuzz_index(fuzz_index < 1.0f ? fuzz_index : 1.0f)
    {
    }
    
    bool Metal::Scatter(const IntersectionData &int_data, Color &attenuation, Ray &scattered_ray) const noexcept {
        const auto reflected = math::Reflect(int_data.casted_ray.direction, int_data.normal);
        scattered_ray = Ray(int_data.point, reflected + m_fuzz_index * math::RandomUnitVector<math::vec3f>());

        attenuation = m_color;

        return math::Dot(reflected, int_data.normal) > 0.0f;
    }
    
    void Metal::SetFuzzIndex(float fuzz_index) noexcept {
        m_fuzz_index = fuzz_index;
    }
    
    float Metal::GetFuzzIndex() noexcept {
        return m_fuzz_index;
    }
}
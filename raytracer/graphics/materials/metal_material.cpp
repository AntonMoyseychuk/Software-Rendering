#include "metal_material.hpp"

#include "graphics/intersection_data.hpp"
#include "math_3d/vector_operations.hpp"

namespace gfx {
    Metal::Metal(Color color)
        : IColoredMaterial(color)
    {
    }
    
    bool Metal::Scatter(const IntersectionData &int_data, Color &attenuation, Ray &scattered_ray) const noexcept {
        const auto reflected = math::Reflect(int_data.casted_ray.direction, int_data.normal);
        scattered_ray = Ray(int_data.point, reflected);

        attenuation = m_color;

        return math::Dot(reflected, int_data.normal) > 0.0f;
    }
}
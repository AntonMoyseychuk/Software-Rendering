#include "diffuse_material.hpp"
#include "graphics/intersection_data.hpp"

namespace gfx {
    Diffuse::Diffuse(Color color)
        : IColoredMaterial(color)
    {
    }
    
    bool Diffuse::Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept {
        const auto scattered_ray_dir = math::Normalize(int_data.normal + math::RandomUnitVectorInHemisphere(int_data.normal));
        scattered_ray = Ray(int_data.point, math::IsTendsTo(scattered_ray_dir.Length(), 0.0f) ? int_data.normal : scattered_ray_dir);

        attenuation = m_color;
        return true;
    }
}
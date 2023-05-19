#include "diffuse_material.hpp"
#include "graphics/intersection_data.hpp"

#include "math_3d/math.hpp"

namespace raytracing::gfx {
    Diffuse::Diffuse(const math::color& color)
        : IColoredMaterial(color)
    {
    }
    
    bool Diffuse::Scatter(const IntersectionData& int_data, math::color& attenuation, Ray& scattered_ray) const noexcept {
        const auto scattered_ray_dir = math::normalize(int_data.normal + math::random_unit_vector_in_hemisphere(int_data.normal));
        scattered_ray = Ray(int_data.point, math::IsTendsTo(scattered_ray_dir.length(), 0.0f) ? int_data.normal : scattered_ray_dir);

        attenuation = color;
        return true;
    }
}
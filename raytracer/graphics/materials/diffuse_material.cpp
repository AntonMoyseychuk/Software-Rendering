#include "diffuse_material.hpp"
#include "graphics/intersection_data.hpp"

#include "math_3d/math.hpp"

namespace raytracing::gfx {
    Diffuse::Diffuse(const math::color& color)
        : IColoredMaterial(color)
    {
    }
    
    bool Diffuse::Scatter(const IntersectionData& int_data, math::color& attenuation, Ray& scattered_ray) const noexcept {
        using namespace math;
        const auto scattered_ray_dir = normalize(int_data.normal + vec3(random_unit_vector_in_hemisphere(vec4(int_data.normal))));
        scattered_ray = Ray(int_data.point, is_tends_to(scattered_ray_dir.length(), 0.0f) ? int_data.normal : scattered_ray_dir);

        attenuation = color;
        return true;
    }
}
#include "dielectric_material.hpp"

#include "math_3d/math.hpp"

#include "graphics/intersection_data.hpp"

namespace raytracing::gfx {
    Dielectric::Dielectric(float _refraction_index)
        : IMaterial(), refraction_index(_refraction_index >= 0.0f ? _refraction_index : 0.0f)
    {
    }
    
    bool Dielectric::Scatter(const IntersectionData &int_data, math::color &attenuation, Ray &scattered_ray) const noexcept {
        const float refraction_ratio = int_data.is_front_face ? (1.0f / refraction_index) : refraction_index;

        float cos_theta = std::fmin(math::dot(-int_data.casted_ray.direction, int_data.normal), 1.0f);
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
        math::vec3 direction = (cannot_refract || _Reflectance(cos_theta, refraction_ratio) > math::random(0.0f, 1.0f)) ? 
            math::reflect(int_data.casted_ray.direction, int_data.normal) : 
            math::refract(int_data.casted_ray.direction, int_data.normal, refraction_ratio);

        scattered_ray = Ray(int_data.point, direction);
        attenuation = math::color::WHITE;
        return true;
    }
    
    float Dielectric::_Reflectance(float cosine, float refraction_ratio) noexcept {
        auto r0 = (1.0f - refraction_ratio) / (1.0f + refraction_ratio);
        r0 *= r0;
        return r0 + (1.0f - r0) * std::powf((1.0f - cosine), 5.0f);
    }
}
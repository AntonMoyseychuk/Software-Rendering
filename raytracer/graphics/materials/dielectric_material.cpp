#include "dielectric_material.hpp"
#include "graphics/color.hpp"
#include "graphics/intersection_data.hpp"

namespace gfx {
    Dielectric::Dielectric(float refraction_index)
        : IMaterial(), m_refraction_index(refraction_index >= 0.0f ? refraction_index : 0.0f)
    {
    }
    
    bool Dielectric::Scatter(const IntersectionData &int_data, Color &attenuation, Ray &scattered_ray) const noexcept {
        const float refraction_ratio = int_data.is_front_face ? (1.0f / m_refraction_index) : m_refraction_index;

        float cos_theta = std::fmin(math::Dot(-int_data.casted_ray.direction, int_data.normal), 1.0f);
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        math::vec3f direction = (cannot_refract || _Reflectance(cos_theta, refraction_ratio) > math::Random(0.0f, 1.0f)) ? 
            math::Reflect(int_data.casted_ray.direction, int_data.normal) : 
            math::Refract(int_data.casted_ray.direction, int_data.normal, refraction_ratio);

        scattered_ray = Ray(int_data.point, direction);
        attenuation = Color(1);
        return true;
    }
    
    void Dielectric::SetRefractionIndex(float refraction_index) noexcept {
        m_refraction_index = refraction_index >= 0.0f ? refraction_index : 0.0f;
    }
    
    float Dielectric::GetRefractionIndex() noexcept {
        return m_refraction_index;
    }
    
    float Dielectric::_Reflectance(float cosine, float refraction_ratio) noexcept {
        auto r0 = (1.0f - refraction_ratio) / (1.0f + refraction_ratio);
        r0 *= r0;
        return r0 + (1.0f - r0) * std::powf((1.0f - cosine), 5.0f);
    }
}
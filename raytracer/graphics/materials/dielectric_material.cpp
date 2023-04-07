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

        const auto refracted = math::Refract(int_data.casted_ray.direction, int_data.normal, refraction_ratio);

        scattered_ray = Ray(int_data.point, refracted);
        attenuation = Color(1,1,1);
        return true;
    }
    
    void Dielectric::SetRefractionIndex(float refraction_index) noexcept {
        m_refraction_index = refraction_index >= 0.0f ? refraction_index : 0.0f;
    }
    
    float Dielectric::GetRefractionIndex() noexcept {
        return m_refraction_index;
    }
}
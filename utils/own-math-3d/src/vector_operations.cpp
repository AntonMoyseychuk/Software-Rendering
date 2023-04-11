#include "../include/math_3d/vector_operations.hpp"

namespace math {
    vec3f RandomUnitVector() noexcept {
        return Normalize(RandomVector<vec3f>(-1.0f, 1.0f));
    }
    
    vec3f RandomUnitVectorInHemisphere(const vec3f& normal) noexcept {
        const auto unit_vec = RandomUnitVector();
        return Dot(unit_vec, normal) > 0.0f ? unit_vec : -unit_vec;
    }

    float math::Dot(const vec3f& a, const vec3f& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    vec3f Cross(const vec3f& a, const vec3f& b) noexcept {
        return vec3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    vec3f math::Reflect(const vec3f &unit_vec, const vec3f &normal) noexcept {
        const auto L = -unit_vec;
        return 2.0f * normal * math::Dot(L, normal) - L;
    }
    
    vec3f Refract(const vec3f &unit_vec, const vec3f &normal, float n1_over_n2) noexcept {
        const auto cos_alpha = std::fmin(math::Dot(-unit_vec, normal), 1.0f);
        
        const vec3f perp =  n1_over_n2 * (unit_vec + cos_alpha * normal);
        const auto perp_length = perp.Length();

        const vec3f parallel = -std::sqrt(math::Abs(1.0f - perp_length * perp_length)) * normal;

        return perp + parallel;
    }

    float Angle(const vec3f &unit_vec1, const vec3f &unit_vec2) noexcept {
        return acosf(Dot(unit_vec1, unit_vec2));
    }
}
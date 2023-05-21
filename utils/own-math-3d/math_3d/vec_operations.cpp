#include "vec_operations.hpp"
#include "vec4.hpp"
#include "vec3.hpp"

#include <cmath>

namespace math {
    vec4f cross(const vec4f &a, const vec4f &b) noexcept {
        const __m128 a1 = _mm_shuffle_ps(a.mm_128, a.mm_128, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 b1 = _mm_shuffle_ps(b.mm_128, b.mm_128, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 a2 = _mm_shuffle_ps(a.mm_128, a.mm_128, _MM_SHUFFLE(3, 1, 0, 2));
        const __m128 b2 = _mm_shuffle_ps(b.mm_128, b.mm_128, _MM_SHUFFLE(3, 1, 0, 2));
        const __m128 t1 = _mm_mul_ps(a2, b1);
        const __m128 t2 = _mm_mul_ps(a1, b2);
        return vec4f(_mm_sub_ps(t2, t1));
    }
    
    float dot(const vec4f &a, const vec4f &b) noexcept {
        return _mm_cvtss_f32(_mm_dp_ps(a.mm_128, b.mm_128, 0xF1));
    }

    vec3f cross(const vec3f &a, const vec3f &b) noexcept {
        const __m128 a_mm_128 = _mm_set_ps(0.0f, a.z, a.y, a.x);
        const __m128 b_mm_128 = _mm_set_ps(0.0f, b.z, b.y, b.x);

        const __m128 a1 = _mm_shuffle_ps(a_mm_128, a_mm_128, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 b1 = _mm_shuffle_ps(b_mm_128, b_mm_128, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 a2 = _mm_shuffle_ps(a_mm_128, a_mm_128, _MM_SHUFFLE(3, 1, 0, 2));
        const __m128 b2 = _mm_shuffle_ps(b_mm_128, b_mm_128, _MM_SHUFFLE(3, 1, 0, 2));
        const __m128 t1 = _mm_mul_ps(a2, b1);
        const __m128 t2 = _mm_mul_ps(a1, b2);

        float arr[4];
        const __m128 result = _mm_sub_ps(t2, t1);
        _mm_store_ps(arr, result);
        
        return vec3f(arr);
    }

    float dot(const vec3f &a, const vec3f &b) noexcept {
        const __m128 a_mm_128 = _mm_set_ps(0.0f, a.z, a.y, a.x);
        const __m128 b_mm_128 = _mm_set_ps(0.0f, b.z, b.y, b.x);

        return _mm_cvtss_f32(_mm_dp_ps(a_mm_128, b_mm_128, 0xF1));
    }

    vec4f random_unit_vector() noexcept {
        return normalize(vec4f::get_random_vector(-1.0f, 1.0f));
    }
    
    vec4f random_unit_vector_in_hemisphere(const vec4f &normal) noexcept {
        const auto unit_vec = random_unit_vector();
        return dot(unit_vec, normal) > 0.0f ? unit_vec : -unit_vec;
    }
    
    vec4f reflect(const vec4f &unit_vec, const vec4f &normal) noexcept {
        const auto L = -unit_vec;
        return 2.0f * normal * dot(L, normal) - L;
    }

    vec3f reflect(const vec3f &unit_vec, const vec3f &normal) noexcept {
        const auto L = -unit_vec;
        return 2.0f * normal * dot(L, normal) - L;
    }

    vec4f refract(const vec4f &unit_vec, const vec4f &normal, float n1_over_n2) noexcept {
        const float cos_alpha = std::fmin(dot(-unit_vec, normal), 1.0f);
        
        const vec4f perp =  n1_over_n2 * (unit_vec + cos_alpha * normal);
        const float perp_length = perp.length();

        const vec4f parallel = -std::sqrt(std::abs(1.0f - perp_length * perp_length)) * normal;

        return perp + parallel;
    }

    vec3f refract(const vec3f &unit_vec, const vec3f &normal, float n1_over_n2) noexcept {
        const float cos_alpha = std::fmin(dot(-unit_vec, normal), 1.0f);
        
        const vec3f perp =  n1_over_n2 * (unit_vec + cos_alpha * normal);
        const float perp_length = perp.length();

        const vec3f parallel = -std::sqrt(std::abs(1.0f - perp_length * perp_length)) * normal;

        return perp + parallel;
    }

    float angle(const vec4f &unit_vec1, const vec4f &unit_vec2) noexcept {
        return std::acosf(dot(unit_vec1, unit_vec2));
    }
    
    float angle(const vec3f &unit_vec1, const vec3f &unit_vec2) noexcept {
        return std::acosf(dot(unit_vec1, unit_vec2));
    }
}
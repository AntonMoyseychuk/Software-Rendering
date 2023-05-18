#include "vec_operations.hpp"

#include "vec4.hpp"

#include <cmath>

namespace math {
    vec4 cross(const vec4 &a, const vec4 &b) noexcept {
        __m128 a1 = _mm_shuffle_ps(a.mm_128, a.mm_128, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 b1 = _mm_shuffle_ps(b.mm_128, b.mm_128, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 a2 = _mm_shuffle_ps(a.mm_128, a.mm_128, _MM_SHUFFLE(3, 1, 0, 2));
        __m128 b2 = _mm_shuffle_ps(b.mm_128, b.mm_128, _MM_SHUFFLE(3, 1, 0, 2));
        __m128 t1 = _mm_mul_ps(a2, b1);
        __m128 t2 = _mm_mul_ps(a1, b2);
        return vec4(_mm_sub_ps(t2, t1));
    }
    
    float dot(const vec4 &a, const vec4 &b) noexcept {
        return _mm_cvtss_f32(_mm_dp_ps(a.mm_128, b.mm_128, 0xF1));
    }
    
    vec4 random_unit_vector() noexcept {
        return normalize(vec4::get_random_in_range(-1.0f, 1.0f));
    }
    
    vec4 random_unit_vector_in_hemisphere(const vec4 &normal) noexcept {
        const auto unit_vec = random_unit_vector();
        return dot(unit_vec, normal) > 0.0f ? unit_vec : -unit_vec;
    }
    
    vec4 reflect(const vec4 &unit_vec, const vec4 &normal) noexcept {
        const auto L = -unit_vec;
        return 2.0f * normal * dot(L, normal) - L;
    }
    
    vec4 refract(const vec4 &unit_vec, const vec4 &normal, float n1_over_n2) noexcept {
        const float cos_alpha = std::fmin(dot(-unit_vec, normal), 1.0f);
        
        const vec4 perp =  n1_over_n2 * (unit_vec + cos_alpha * normal);
        const float perp_length = perp.length();

        const vec4 parallel = -std::sqrt(std::abs(1.0f - perp_length * perp_length)) * normal;

        return perp + parallel;
    }
    
    float angle(const vec4 &unit_vec1, const vec4 &unit_vec2) noexcept {
        return std::acosf(dot(unit_vec1, unit_vec2));
    }
}
#pragma once
#include <type_traits>

namespace math {
    union vec4;

    vec4 cross(const vec4& a, const vec4& b) noexcept;
    float dot(const vec4& a, const vec4& b) noexcept;

    vec4 random_unit_vector() noexcept;
    vec4 random_unit_vector_in_hemisphere(const vec4& normal) noexcept;

    vec4 reflect(const vec4& unit_vec, const vec4& normal) noexcept;
    vec4 refract(const vec4& unit_vec, const vec4& normal, float n1_over_n2) noexcept;

    float angle(const vec4& unit_vec1, const vec4& unit_vec2) noexcept;
}
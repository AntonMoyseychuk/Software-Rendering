#pragma once
#include "vec4.hpp"

namespace math {
    vec3f barycentric(const vec3f& a, const vec3f& b, const vec3f& c, const vec3f& point) noexcept;

    vec4f cross(const vec4f& a, const vec4f& b) noexcept;
    float dot(const vec4f& a, const vec4f& b) noexcept;

    vec3f cross(const vec3f& a, const vec3f& b) noexcept;
    float dot(const vec3f& a, const vec3f& b) noexcept;

    vec4f random_unit_vector() noexcept;
    vec4f random_unit_vector_in_hemisphere(const vec4f& normal) noexcept;

    vec4f reflect(const vec4f& unit_vec, const vec4f& normal) noexcept;
    vec3f reflect(const vec3f& unit_vec, const vec3f& normal) noexcept;
    
    vec4f refract(const vec4f& unit_vec, const vec4f& normal, float n1_over_n2) noexcept;
    vec3f refract(const vec3f& unit_vec, const vec3f& normal, float n1_over_n2) noexcept;

    float angle(const vec4f& unit_vec1, const vec4f& unit_vec2) noexcept;
    float angle(const vec3f& unit_vec1, const vec3f& unit_vec2) noexcept;
}
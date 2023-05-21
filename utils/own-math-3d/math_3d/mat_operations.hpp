#pragma once
#include "mat4.hpp"

namespace math {
    union vec4f;

    mat4f scale(const mat4f& mat, const vec4f& scale) noexcept;
    mat4f translate(const mat4f& mat, const vec4f& translation) noexcept;
    
    mat4f rotate_x(const mat4f& mat, float angle_radians) noexcept;
    mat4f rotate_y(const mat4f& mat, float angle_radians) noexcept;
    mat4f rotate_z(const mat4f& mat, float angle_radians) noexcept;

    mat4f look_at_rh(const vec4f& eye, const vec4f& look_at, const vec4f& up) noexcept;
    mat4f perspective(float fovy_radians, float aspect, float z_near, float z_far) noexcept;
}
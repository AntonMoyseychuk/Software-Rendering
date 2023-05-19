#pragma once
#include "mat4.hpp"

namespace math {
    union vec4;

    mat4 scale(const mat4& mat, const vec4& scale) noexcept;
    mat4 translate(const mat4& mat, const vec4& translation) noexcept;
    
    mat4 rotate_x(const mat4& mat, float angle_radians) noexcept;
    mat4 rotate_y(const mat4& mat, float angle_radians) noexcept;
    mat4 rotate_z(const mat4& mat, float angle_radians) noexcept;

    mat4 look_at_rh(const vec4& eye, const vec4& look_at, const vec4& up) noexcept;
    mat4 perspective(float fovy_radians, float aspect, float z_near, float z_far) noexcept;
}
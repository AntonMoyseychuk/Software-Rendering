#pragma once

namespace math {
    union vec4;
    union vec3;
    union vec2;

    struct quaternion {
        quaternion() = default;
        quaternion(float w, float x, float y, float z);

        quaternion operator*(const quaternion& q) const noexcept;

        float w, x, y, z;
    };

    quaternion normalize(const quaternion& q) noexcept;
    quaternion conjugate(const quaternion& q) noexcept;

    vec4 operator*(const vec4& vec, const quaternion& q) noexcept;
    vec4& operator*=(vec4& vec, const quaternion& q) noexcept;

    vec3 operator*(const vec3& vec, const quaternion& q) noexcept;
    vec3& operator*=(vec3& vec, const quaternion& q) noexcept;

    vec2 operator*(const vec2& vec, const quaternion& q) noexcept;
    vec2& operator*=(vec2& vec, const quaternion& q) noexcept;
}

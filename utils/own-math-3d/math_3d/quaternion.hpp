#pragma once

namespace math {
    union vec4f;
    union vec3f;
    union vec2f;

    struct quaternion {
        quaternion() = default;
        quaternion(float w, float x, float y, float z);

        quaternion operator*(const quaternion& q) const noexcept;

        float w, x, y, z;
    };

    quaternion normalize(const quaternion& q) noexcept;
    quaternion conjugate(const quaternion& q) noexcept;

    vec4f operator*(const vec4f& vec, const quaternion& q) noexcept;
    vec4f& operator*=(vec4f& vec, const quaternion& q) noexcept;

    vec3f operator*(const vec3f& vec, const quaternion& q) noexcept;
    vec3f& operator*=(vec3f& vec, const quaternion& q) noexcept;

    vec2f operator*(const vec2f& vec, const quaternion& q) noexcept;
    vec2f& operator*=(vec2f& vec, const quaternion& q) noexcept;
}

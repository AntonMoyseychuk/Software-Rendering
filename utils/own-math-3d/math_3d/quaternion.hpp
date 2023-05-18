#pragma once

namespace math {
    union vec4;

    struct quaternion {
        quaternion() = default;
        quaternion(float w, float x, float y, float z);

        quaternion operator*(const quaternion& q) const noexcept;

        float w, x, y, z;
    };

    quaternion normalize(const quaternion& q) noexcept;
    quaternion conjugate(const quaternion& q) noexcept;

    vec4 operator*(const vec4& vec, const quaternion& q) noexcept;
}

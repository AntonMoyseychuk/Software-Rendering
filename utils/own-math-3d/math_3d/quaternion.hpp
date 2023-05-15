#pragma once

namespace math {
    struct Quaternion {
        Quaternion() = default;
        Quaternion(float w, float x, float y, float z);

        Quaternion operator*(const Quaternion& q) const noexcept;

        float w, x, y, z;
    };

    Quaternion Normalize(const Quaternion& q) noexcept;
    Quaternion Conjugate(const Quaternion& q) noexcept;
}

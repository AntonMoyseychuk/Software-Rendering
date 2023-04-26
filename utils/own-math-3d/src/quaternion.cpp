#include "../include/math_3d/quaternion.hpp"

#include <cmath>

namespace math {
    math::Quaternion::Quaternion(float _w, float _x, float _y, float _z)
        : w(_w), x(_x), y(_y), z(_z)
    {
    }
    
    Quaternion Quaternion::operator*(const Quaternion &q) const noexcept {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }
    
    Quaternion Normalize(const Quaternion &q) noexcept {
        const auto norm = std::sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
        return Quaternion(q.w / norm, q.x / norm, q.y / norm, q.z / norm);
    }
    
    Quaternion Conjugate(const Quaternion &q) noexcept {
        return Quaternion(q.w, -q.x, -q.y, -q.z);
    }
}
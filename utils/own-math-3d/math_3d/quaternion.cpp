#include "quaternion.hpp"

namespace math {
    math::quaternion::quaternion(float _w, float _x, float _y, float _z)
        : w(_w), x(_x), y(_y), z(_z)
    {
    }
    
    quaternion quaternion::operator*(const quaternion &q) const noexcept {
        return quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    bool quaternion::operator==(const quaternion &q) const noexcept {
        return is_tends_to(x, q.x) && is_tends_to(y, q.y) && is_tends_to(z, q.z) && is_tends_to(w, q.w);
    }

    bool quaternion::operator!=(const quaternion &q) const noexcept {
        return !this->operator==(q);
    }

    quaternion normalize(const quaternion &q) noexcept {
        const auto norm = std::sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
        return quaternion(q.w / norm, q.x / norm, q.y / norm, q.z / norm);
    }
    
    quaternion conjugate(const quaternion &q) noexcept {
        return quaternion(q.w, -q.x, -q.y, -q.z);
    }
    
    vec4f operator*(const vec4f &vec, const quaternion &q) noexcept {
        const quaternion rotated = q * quaternion(0.0f, vec.x, vec.y, vec.z) * conjugate(q);
        return vec4f(rotated.x, rotated.y, rotated.z, 0.0f);
    }
    
    vec4f& operator*=(vec4f &vec, const quaternion &q) noexcept {
        vec = vec * q;
        return vec;
    }
    
    vec3f operator*(const vec3f &vec, const quaternion &q) noexcept {
        const quaternion rotated = q * quaternion(0.0f, vec.x, vec.y, vec.z) * conjugate(q);
        return vec3f(rotated.x, rotated.y, rotated.z);
    }
    
    vec3f &operator*=(vec3f &vec, const quaternion &q) noexcept {
        vec = vec * q;
        return vec;
    }
    
    vec2f operator*(const vec2f &vec, const quaternion &q) noexcept {
        const quaternion rotated = q * quaternion(0.0f, vec.x, vec.y, 0.0f) * conjugate(q);
        return vec2f(rotated.x, rotated.y);
    }
    
    vec2f &operator*=(vec2f &vec, const quaternion &q) noexcept {
        vec = vec * q;
        return vec;
    }
}
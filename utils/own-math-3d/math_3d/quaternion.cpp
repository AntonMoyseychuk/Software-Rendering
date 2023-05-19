#include "quaternion.hpp"
#include "vec4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"

#include <cmath>

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
    
    quaternion normalize(const quaternion &q) noexcept {
        const auto norm = std::sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
        return quaternion(q.w / norm, q.x / norm, q.y / norm, q.z / norm);
    }
    
    quaternion conjugate(const quaternion &q) noexcept {
        return quaternion(q.w, -q.x, -q.y, -q.z);
    }
    
    vec4 operator*(const vec4 &vec, const quaternion &q) noexcept {
        const quaternion rotated = q * quaternion(0.0f, vec.x, vec.y, vec.z) * conjugate(q);
        return vec4(rotated.x, rotated.y, rotated.z, 0.0f);
    }
    
    vec4& operator*=(vec4 &vec, const quaternion &q) noexcept {
        vec = vec * q;
        return vec;
    }
    
    vec3 operator*(const vec3 &vec, const quaternion &q) noexcept {
        const quaternion rotated = q * quaternion(0.0f, vec.x, vec.y, vec.z) * conjugate(q);
        return vec3(rotated.x, rotated.y, rotated.z);
    }
    
    vec3 &operator*=(vec3 &vec, const quaternion &q) noexcept {
        vec = vec * q;
        return vec;
    }
    
    vec2 operator*(const vec2 &vec, const quaternion &q) noexcept {
        const quaternion rotated = q * quaternion(0.0f, vec.x, vec.y, 0.0f) * conjugate(q);
        return vec2(rotated.x, rotated.y);
    }
    
    vec2 &operator*=(vec2 &vec, const quaternion &q) noexcept {
        vec = vec * q;
        return vec;
    }
}
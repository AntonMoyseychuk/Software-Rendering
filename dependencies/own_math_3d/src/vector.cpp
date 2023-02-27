#include "math_3d/vector.hpp"
#include "math_3d/mat3x3.hpp"

#include <stdexcept>
#include <cassert>


namespace math
{
    Vec3f::Vec3f()
        : Vec3f(0.0f) {}

    Vec3f::Vec3f(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z) {}

    Vec3f::Vec3f(float value)
        : x(value), y(value), z(value) {}

    float Vec3f::Length() const noexcept {
        return sqrtf(x * x + y * y + z * z);
    }

    Vec3f Vec3f::Normalize() const noexcept {
        auto length = this->Length();

        if (length <= EPSILON) {
            return *this;
        }

        return Vec3f(x / length, y / length, z / length);
    }

    Vec3f Vec3f::operator+(const Vec3f &vec) const noexcept {
        return Vec3f(x + vec.x, y + vec.y, z + vec.z);
    }

    Vec3f &Vec3f::operator+=(const Vec3f &vec) noexcept {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    Vec3f Vec3f::operator-(const Vec3f &vec) const noexcept {
        return Vec3f(x - vec.x, y - vec.y, z - vec.z);
    }

    Vec3f &Vec3f::operator-=(const Vec3f &vec) noexcept {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }

    Vec3f Vec3f::operator*(float value) const noexcept {
        return Vec3f(x * value, y * value, z * value);
    }

    Vec3f& Vec3f::operator*=(float value) noexcept {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    Vec3f Vec3f::operator/(float value) const noexcept {
        return Vec3f(x / value, y / value, z / value);
    }

    Vec3f& Vec3f::operator/=(float value) noexcept {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    Vec3f Vec3f::operator-() const noexcept {
        return Vec3f(-x, -y, -z);
    }

    float &Vec3f::operator[](std::size_t idx) {
        if (idx > 2) {
           throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&x + idx);
    }

    const float &Vec3f::operator[](std::size_t idx) const {
        if (idx > 2) {
           throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&x + idx);
    }

    const Vec3f Vec3f::RIGHT(1.0f, 0.0f, 0.0f);
    const Vec3f Vec3f::DOWN(0.0f, -1.0f, 0.0f);
    const Vec3f Vec3f::LEFT(-1.0f, 0.0f, 0.0f);
    const Vec3f Vec3f::UP(0.0f, 1.0f, 0.0f);
    const Vec3f Vec3f::FORWARD(0.0f, 0.0f, 1.0f);
    const Vec3f Vec3f::BACKWARD(0.0f, 0.0f, -1.0f);
}
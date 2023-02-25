#include "math_3d/vector3.hpp"
#include "math_3d/mat3x3.hpp"

#include <stdexcept>
#include <cassert>


namespace math
{
    Vector3f::Vector3f()
        : Vector3f(0.0f) {}

    Vector3f::Vector3f(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z) {}

    Vector3f::Vector3f(float value)
        : x(value), y(value), z(value) {}

    float Vector3f::Length() const noexcept {
        return sqrtf(x * x + y * y + z * z);
    }

    Vector3f Vector3f::Normalize() const noexcept {
        auto length = this->Length();

        if (length <= EPSILON) {
            return *this;
        }

        return Vector3f(x / length, y / length, z / length);
    }

    Vector3f Vector3f::operator+(const Vector3f &vec) const noexcept {
        return Vector3f(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector3f &Vector3f::operator+=(const Vector3f &vec) noexcept {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    Vector3f Vector3f::operator-(const Vector3f &vec) const noexcept {
        return Vector3f(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3f &Vector3f::operator-=(const Vector3f &vec) noexcept {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }

    Vector3f Vector3f::operator*(const Mat3x3 &mat) const noexcept {
        return Vector3f(
            x * mat[0][0] + y * mat[1][0] + z * mat[2][0],
            x * mat[0][1] + y * mat[1][1] + z * mat[2][1],
            x * mat[0][2] + y * mat[1][2] + z * mat[2][2]
        );
    }

    Vector3f Vector3f::operator*(float value) const noexcept {
        return Vector3f(x * value, y * value, z * value);
    }

    Vector3f& Vector3f::operator*=(float value) noexcept {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    Vector3f Vector3f::operator/(float value) const noexcept {
        return Vector3f(x / value, y / value, z / value);
    }

    Vector3f& Vector3f::operator/=(float value) noexcept {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    Vector3f Vector3f::operator-() const noexcept {
        return Vector3f(-x, -y, -z);
    }

    float &Vector3f::operator[](std::size_t idx) {
        if (idx > 2) {
           throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&x + idx);
    }

    const float &Vector3f::operator[](std::size_t idx) const {
        if (idx > 2) {
           throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&x + idx);
    }

    const Vector3f Vector3f::RIGHT(1.0f, 0.0f, 0.0f);
    const Vector3f Vector3f::DOWN(0.0f, -1.0f, 0.0f);
    const Vector3f Vector3f::LEFT(-1.0f, 0.0f, 0.0f);
    const Vector3f Vector3f::UP(0.0f, 1.0f, 0.0f);
    const Vector3f Vector3f::FORWARD(0.0f, 0.0f, 1.0f);
    const Vector3f Vector3f::BACKWARD(0.0f, 0.0f, -1.0f);
}
#include "math_3d/vector.hpp"
#include "math_3d/mat3x3.hpp"

#include <stdexcept>
#include <cassert>


namespace math
{
    Vector3::Vector3()
        : Vector3(0.0f) {}

    Vector3::Vector3(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z) {}

    Vector3::Vector3(float value)
        : x(value), y(value), z(value) {}

    float Vector3::Length() const noexcept {
        return sqrtf(x * x + y * y + z * z);
    }

    Vector3 Vector3::Normalize() const noexcept {
        auto length = this->Length();

        if (length <= EPSILON) {
            return *this;
        }

        return Vector3(x / length, y / length, z / length);
    }

    Vector3 Vector3::operator+(const Vector3 &vec) const noexcept {
        return Vector3(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector3 &Vector3::operator+=(const Vector3 &vec) noexcept {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    Vector3 Vector3::operator-(const Vector3 &vec) const noexcept {
        return Vector3(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3 &Vector3::operator-=(const Vector3 &vec) noexcept {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }

    Vector3 Vector3::operator*(const Mat3x3 &mat) const noexcept {
        return Vector3(
            x * mat[0][0] + y * mat[1][0] + z * mat[2][0],
            x * mat[0][1] + y * mat[1][1] + z * mat[2][1],
            x * mat[0][2] + y * mat[1][2] + z * mat[2][2]
        );
    }

    Vector3 Vector3::operator*(float value) const noexcept {
        return Vector3(x * value, y * value, z * value);
    }

    Vector3& Vector3::operator*=(float value) noexcept {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    Vector3 Vector3::operator/(float value) const noexcept {
        return Vector3(x / value, y / value, z / value);
    }

    Vector3& Vector3::operator/=(float value) noexcept {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    Vector3 Vector3::operator-() const noexcept {
        return Vector3(-x, -y, -z);
    }

    float &Vector3::operator[](std::size_t idx) {
        if (idx > 2) {
           throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&x + idx);
    }

    const float &Vector3::operator[](std::size_t idx) const {
        if (idx > 2) {
           throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&x + idx);
    }

    const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::DOWN(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::BACKWARD(0.0f, 0.0f, -1.0f);
}
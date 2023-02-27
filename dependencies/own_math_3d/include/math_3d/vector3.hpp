#pragma once
#include "declarations.hpp"

#include <type_traits>

namespace math {
    struct Vector3f {
        Vector3f();
        Vector3f(float x, float y, float z = 0.0f);
        Vector3f(float value);

        float Length() const noexcept;
        Vector3f Normalize() const noexcept;

        Vector3f operator+(const Vector3f& vec) const noexcept;
        Vector3f& operator+=(const Vector3f& vec) noexcept;

        Vector3f operator-() const noexcept;
        Vector3f operator-(const Vector3f& vec) const noexcept;
        Vector3f& operator-=(const Vector3f& vec) noexcept;

        Vector3f operator*(const Mat3x3& mat) const noexcept;
        Vector3f operator*(float value) const noexcept;
        Vector3f& operator*=(float value) noexcept;

        Vector3f operator/(float value) const noexcept;
        Vector3f& operator/=(float value) noexcept;

        float& operator[](std::size_t idx);
        const float& operator[](std::size_t idx) const;

        static const Vector3f RIGHT;
        static const Vector3f DOWN;
        static const Vector3f LEFT;
        static const Vector3f UP;
        static const Vector3f FORWARD;
        static const Vector3f BACKWARD;

        float x, y, z;
    };
}
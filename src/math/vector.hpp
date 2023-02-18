#ifndef VECTOR_H
#define VECTOR_H

#include "declarations.hpp"

#include <type_traits>


namespace math {
    class Vector3 {
    public:
        Vector3();
        Vector3(float x, float y, float z = 0.0f);
        Vector3(float value);

        float Length() const noexcept;
        Vector3 Normalize() const noexcept;

        Vector3 operator+(const Vector3& vec) const noexcept;
        Vector3& operator+=(const Vector3& vec) noexcept;

        Vector3 operator-(const Vector3& vec) const noexcept;
        Vector3& operator-=(const Vector3& vec) noexcept;

        Vector3 operator*(const Mat3x3& mat) const noexcept;
        Vector3 operator*(float value) const noexcept;
        Vector3& operator*=(float value) noexcept;

        Vector3 operator-() const noexcept;

        float& X() noexcept;
        const float& X() const noexcept;

        float& Y() noexcept;
        const float& Y() const noexcept;

        float& Z() noexcept;
        const float& Z() const noexcept;

        float& operator[](std::size_t idx);
        const float& operator[](std::size_t idx) const;

    public:
        static const Vector3 RIGHT;
        static const Vector3 DOWN;
        static const Vector3 LEFT;
        static const Vector3 UP;
        static const Vector3 FORWARD;
        static const Vector3 BACKWARD;

    private:
        float m_x, m_y, m_z;
    };
}

#endif
#pragma once
#include <type_traits>

namespace math {
    struct Vec3f {
        Vec3f();
        Vec3f(float x, float y, float z = 0.0f);
        Vec3f(float value);

        float Length() const noexcept;
        Vec3f Normalize() const noexcept;

        Vec3f operator+(const Vec3f& vec) const noexcept;
        Vec3f& operator+=(const Vec3f& vec) noexcept;

        Vec3f operator-() const noexcept;
        Vec3f operator-(const Vec3f& vec) const noexcept;
        Vec3f& operator-=(const Vec3f& vec) noexcept;

        Vec3f operator*(float value) const noexcept;
        Vec3f& operator*=(float value) noexcept;

        Vec3f operator/(float value) const noexcept;
        Vec3f& operator/=(float value) noexcept;

        float& operator[](std::size_t idx);
        const float& operator[](std::size_t idx) const;

        static const Vec3f RIGHT;
        static const Vec3f DOWN;
        static const Vec3f LEFT;
        static const Vec3f UP;
        static const Vec3f FORWARD;
        static const Vec3f BACKWARD;

        float x, y, z;
    };
}
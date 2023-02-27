#pragma once
#include "declarations.hpp"

namespace math {
    struct Mat3x3 {
        Mat3x3();
        Mat3x3(const Vec3f &r0, const Vec3f &r1, const Vec3f &r2);

        void SetIdentity() noexcept;
        float Determinant() const noexcept;

        Mat3x3 operator*(const Mat3x3 &m) const noexcept;
        Mat3x3 operator*(float value) const noexcept;

        float *operator[](std::size_t idx);
        const float *operator[](std::size_t idx) const;

        static const std::size_t order = 3;
        float solid_array[order][order];
    };
}
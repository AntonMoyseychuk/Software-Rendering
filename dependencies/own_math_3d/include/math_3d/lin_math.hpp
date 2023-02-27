#pragma once
#include "declarations.hpp"

namespace math {
    class LinMath final {
    public:
        LinMath() = delete;
        LinMath(const LinMath& other) = delete;
        LinMath(LinMath&& other) = delete;
        LinMath& operator=(const LinMath& other) = delete;
        LinMath& operator=(LinMath&& other) = delete;

        static float Dot(const Vec3f& a, const Vec3f& b) noexcept;
        static Vec3f Cross(const Vec3f& a, const Vec3f& b) noexcept;

        static Vec3f Mult(const Vec3f vec, const Mat3x3& mat) noexcept;

        static Mat3x3 Transpose(const Mat3x3& mat) noexcept;
        static Mat3x3 Inverse(const Mat3x3& mat);
    };
    
}
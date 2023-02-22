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

        static float Dot(const Vector3& a, const Vector3& b) noexcept;
        static Vector3 Cross(const Vector3& a, const Vector3& b) noexcept;

        static Mat3x3 Transpose(const Mat3x3& mat) noexcept;
        static Mat3x3 Inverse(const Mat3x3& mat);
    };
    
}
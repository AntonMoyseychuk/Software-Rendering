#pragma once
#include "math_3d/vector.hpp"
#include "math_3d/mat3x3.hpp"

#include <cmath>


namespace math {
    class LinMath final {
    public:
        LinMath() = delete;
        LinMath(const LinMath& other) = delete;
        LinMath(LinMath&& other) = delete;
        LinMath& operator=(const LinMath& other) = delete;
        LinMath& operator=(LinMath&& other) = delete;

        template<typename _LeftType, typename _RightType>
        static auto Dot(const Vector<_LeftType>& a, const Vector<_RightType>& b) noexcept -> decltype(a.x * b.x) {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        //only for 3D vectors
        template<typename _LeftType, typename _RightType>
        static auto Cross(const Vector<_LeftType>& a, const Vector<_RightType>& b) noexcept -> Vector<decltype(a.x * b.x)> {
            return Vector<decltype(a.x * b.x)>(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        template<typename _VecType>
        static auto Mult(const Vector<_VecType>& vec, const Mat3x3& mat) noexcept -> Vector<decltype(vec.x * mat[0][0])> {
            return Vector(
                vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0],
                vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1],
                vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2]
            );
        }

        static float ToDegrees(float radians) noexcept; 
        static float ToRadians(float degrees) noexcept; 

        static Mat3x3 Transpose(const Mat3x3& mat) noexcept;
        static bool Inverse(const Mat3x3& mat, Mat3x3& out);

        template<typename _Type>
        static bool IsTendsTo(const _Type& value, const _Type& limit) {
            return std::abs(limit - value) <= EPSILON;
        }
    };

    constexpr auto EPSILON = 1e-6;
    constexpr auto PI = 3.14159265359;
}
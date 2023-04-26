#pragma once
#include "vector.hpp"
#include "quaternion.hpp"
#include "matrix_operations.hpp"

namespace math {
    vec3f RandomUnitVector() noexcept;
    vec3f RandomUnitVectorInHemisphere(const vec3f& normal) noexcept;

    float Dot(const vec3f& a, const vec3f& b) noexcept;
    vec3f Cross(const vec3f& a, const vec3f& b) noexcept;

    vec3f Reflect(const vec3f& unit_vec, const vec3f& normal) noexcept;
    vec3f Refract(const vec3f& unit_vec, const vec3f& normal, float n1_over_n2) noexcept;

    float Angle(const vec3f& unit_vec1, const vec3f& unit_vec2) noexcept;

    template <typename Type>
    vec4f Normalize(const Vector<Type, 4>& vec) noexcept {
        auto length = vec.Length();

        if (math::IsTendsTo(length, 0.0f)) {
            return vec;
        }

        return vec4f(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
    }

    template <typename Type>
    vec3f Normalize(const Vector<Type, 3>& vec) noexcept {
        auto length = vec.Length();

        if (math::IsTendsTo(length, 0.0f)) {
            return vec;
        }

        return vec3f(vec.x / length, vec.y / length, vec.z / length);
    }

    template <typename Type>
    vec2f Normalize(const Vector<Type, 2>& vec) noexcept {
        auto length = vec.Length();

        if (math::IsTendsTo(length, 0.0f)) {
            return vec;
        }

        return vec2f(vec.x / length, vec.y / length);
    }

    template<typename Vec, typename MatrixType>
    inline vec4f operator*(const Vec& vec, const Mat<MatrixType, 4>& mat) noexcept {
        static_assert(std::is_arithmetic_v<typename Vec::type> && (Vec::dimension >= 2 && Vec::dimension <= 4), 
            "template<typename Vec, typename MatrixType> inline vec4f operator*(const Vec& vec, const Mat<MatrixType, 4>& mat): Invalid Vec type");

        const vec4f temp_vec(vec);
        return vec4f(
            temp_vec.x * mat[0][0] + temp_vec.y * mat[1][0] + temp_vec.z * mat[2][0] + temp_vec.w * mat[3][0],
            temp_vec.x * mat[0][1] + temp_vec.y * mat[1][1] + temp_vec.z * mat[2][1] + temp_vec.w * mat[3][1],
            temp_vec.x * mat[0][2] + temp_vec.y * mat[1][2] + temp_vec.z * mat[2][2] + temp_vec.w * mat[3][2],
            temp_vec.x * mat[0][3] + temp_vec.y * mat[1][3] + temp_vec.z * mat[2][3] + temp_vec.w * mat[3][3]
        );
    }

    template<typename Type>
    inline vec3f operator*(const Vector<Type, 3>& vec, const Quaternion& q) noexcept {
        const auto rotated = q * Quaternion(0, vec.x, vec.y, vec.z) * Conjugate(q);
        return vec3f(rotated.x, rotated.y, rotated.z);
    }
}

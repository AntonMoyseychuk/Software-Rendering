#pragma once
#include "vec3.hpp"
#include "vec4.hpp"

namespace math {
    union mat4 {
        mat4() noexcept = default;
        mat4(float v00, float v01, float v02, float v03,
            float v04, float v05, float v06, float v07,
            float v08, float v09, float v10, float v11,
            float v12, float v13, float v14, float v15
        ) noexcept;
        mat4(const vec4& vec_row0, const vec4& vec_row1, const vec4& vec_row2, const vec4& vec_row3) noexcept;
        mat4(const __m128& row0, const __m128& row1, const __m128& row2, const __m128& row3) noexcept;
        mat4(const mat4& mat) noexcept;

        mat4& operator=(const mat4& mat) noexcept;

        mat4 operator-() const noexcept;

        mat4 operator+(const mat4& mat) const noexcept; 
        mat4& operator+=(const mat4& mat) noexcept; 
        mat4 operator-(const mat4& mat) const noexcept; 
        mat4& operator-=(const mat4& mat) noexcept;

        mat4 operator*(float value) const noexcept; 
        mat4& operator*=(float value) noexcept; 
        mat4 operator/(float value) const noexcept; 
        mat4& operator/=(float value) noexcept;

        mat4 operator*(const mat4& mat) const noexcept; 
        mat4& operator*=(const mat4& mat) noexcept; 

        vec4& operator[](size_t i) noexcept;
        const vec4& operator[](size_t i) const noexcept;

        float determinant() const noexcept;

        const inline static size_t raw_count = 4;
        const inline static size_t column_count = 4;

        static const mat4 IDENTITY;

        struct {
            float v00, v01, v02, v03;
            float v04, v05, v06, v07;
            float v08, v09, v10, v11;
            float v12, v13, v14, v15;
        };

        struct {
            vec4 vec_row0;
            vec4 vec_row1;
            vec4 vec_row2;
            vec4 vec_row3;
        };

        struct {
            __m128 mm_row0;
            __m128 mm_row1;
            __m128 mm_row2;
            __m128 mm_row3;
        };

        float arr[16];
        vec4 vec_arr[4];
        __m128 mm_arr[4];
    };

    mat4 operator*(float value, const mat4 mat) noexcept;
    vec3 operator*(const vec3& vec, const mat4 mat) noexcept;
    vec4 operator*(const vec4& vec, const mat4 mat) noexcept;

    mat4 inverse(const mat4& mat) noexcept;
    mat4 transpose(const mat4& mat) noexcept;
}
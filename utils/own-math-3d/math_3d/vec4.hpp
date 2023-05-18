#pragma once
#include "vec2.hpp"
#include "vec3.hpp"

#include <immintrin.h>

namespace math {
    struct quaternion;

    union vec4;
    using color = vec4;

    union vec4 {
        vec4() noexcept = default;
        vec4(float value) noexcept;
        vec4(const __m128& mm_128) noexcept;
        vec4(float x, float y, float z, float w) noexcept;

        vec4(const vec2& vec) noexcept;
        vec4(const vec2& xy, const vec2& zw) noexcept;
        vec4(const vec2& xy, float z, float w) noexcept;
        vec4(float x, const vec2& yz, float w) noexcept;
        vec4(float x, float y, const vec2& zw) noexcept;

        vec4(const vec3& vec) noexcept;
        vec4(const vec3& xyz, float w) noexcept;
        vec4(float x, const vec3& yzw) noexcept;

        vec4& operator=(const vec2& vec) noexcept;
        vec4& operator=(const vec3& vec) noexcept;

        vec4 operator-() const noexcept;

        vec4 operator+(const vec4& vec) const noexcept;
        vec4 operator-(const vec4& vec) const noexcept;
        vec4& operator+=(const vec4& vec) noexcept;
        vec4& operator-=(const vec4& vec) noexcept;

        vec4 operator*(float value) const noexcept;
        vec4& operator*=(float value) noexcept;
        vec4 operator/(float value) const noexcept;
        vec4& operator/=(float value) noexcept;

        vec4 operator*(const quaternion& q) const noexcept;
        vec4& operator*=(const quaternion& q) noexcept;

        bool operator==(const vec4& vec) const noexcept;
        bool operator!=(const vec4& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec4 get_random_in_range(float min, float max) noexcept;

        static const vec4 left; 
        static const vec4 up; 
        static const vec4 right; 
        static const vec4 down; 
        static const vec4 forward; 
        static const vec4 backward;

        static const color red;
        static const color green;
        static const color blue;
        static const color yellow;
        static const color magenta;
        static const color cyan;
        static const color orange;
        static const color lime;
        static const color purple;
        static const color indigo;
        static const color golden;
        static const color sky_blue;
        static const color white;
        static const color black;
        static const color grey;

        struct {
            float x, y, z, w;
        };

        struct {
            vec2 xy;
            float _ignore_0;
            float _ignore_1;
        };
        
        struct {
            float _ignore_2;
            vec2 yz;
            float _ignore_3;
        };

        struct {
            float _ignore_4;
            float _ignore_5;
            vec2 zw;
        };

        struct {
            vec3 xyz;
            float _ignore_6;
        };

        struct {
            float _ignore_7;
            vec3 yzw;
        };

        struct {
            float r, g, b, a;
        };

        struct {
            vec2 rg;
            float _ignore_8;
            float _ignore_9;
        };
        
        struct {
            float _ignore_10;
            vec2 gb;
            float _ignore_11;
        };

        struct {
            float _ignore_12;
            float _ignore_13;
            vec2 ba;
        };

        struct {
            vec3 rgb;
            float _ignore_14;
        };

        struct {
            float _ignore_15;
            vec3 gba;
        };

        float arr[4];
        
        __m128 mm_128;
    };

    vec4 operator*(float value, const vec4& vec) noexcept;

    vec4 normalize(const vec4& vec) noexcept;

    #define TO_UINT32(color)                                \
    (                                                       \
        (0xff << 24) & (uint32_t((color.r) * 255) << 24) |  \
        (0xff << 16) & (uint32_t((color.g) * 255) << 16) |  \
        (0xff << 8) & (uint32_t((color.b) * 255) << 8) |    \
        (0xff) & uint32_t((color.a) * 255)                  \
    )                                       
}
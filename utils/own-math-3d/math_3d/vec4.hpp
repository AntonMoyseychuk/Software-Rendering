#pragma once
#include "vec2.hpp"
#include "vec3.hpp"

#include <immintrin.h>

namespace math {
    union vec4;
    using color = vec4;

    union vec4 {
        vec4() noexcept = default;
        explicit vec4(float value) noexcept;
        explicit vec4(const __m128& mm_128) noexcept;
        explicit vec4(const float* arr) noexcept;
        vec4(float x, float y, float z, float w) noexcept;
        vec4(const vec4& vec) noexcept;

        explicit vec4(const vec2& vec) noexcept;
        vec4(const vec2& xy, const vec2& zw) noexcept;
        vec4(const vec2& xy, float z, float w) noexcept;
        vec4(float x, const vec2& yz, float w) noexcept;
        vec4(float x, float y, const vec2& zw) noexcept;

        explicit vec4(const vec3& vec) noexcept;
        vec4(const vec3& xyz, float w) noexcept;
        vec4(float x, const vec3& yzw) noexcept;

        vec4& operator=(const vec2& vec) noexcept;
        vec4& operator=(const vec3& vec) noexcept;
        vec4& operator=(const vec4& vec) noexcept;

        vec4 operator-() const noexcept;

        vec4 operator+(const vec4& vec) const noexcept;
        vec4 operator-(const vec4& vec) const noexcept;
        vec4& operator+=(const vec4& vec) noexcept;
        vec4& operator-=(const vec4& vec) noexcept;

        vec4 operator*(const vec4& vec) const noexcept;
        vec4& operator*=(const vec4& vec) noexcept;

        vec4 operator*(float value) const noexcept;
        vec4& operator*=(float value) noexcept;
        vec4 operator/(float value) const noexcept;
        vec4& operator/=(float value) noexcept;

        bool operator==(const vec4& vec) const noexcept;
        bool operator!=(const vec4& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec4 get_random_in_range(float min, float max) noexcept;

        static const vec4 ZERO; 
        static const vec4 LEFT; 
        static const vec4 UP; 
        static const vec4 RIGHT; 
        static const vec4 DOWN; 
        static const vec4 FORWARD; 
        static const vec4 BACKWARD;

        static const color RED;
        static const color GREEN;
        static const color BLUE;
        static const color YELLOW;
        static const color MAGENTA;
        static const color CYAN;
        static const color ORANGE;
        static const color LIME;
        static const color PURPLE;
        static const color INDIGO;
        static const color GOLDEN;
        static const color SKY_BLUE;
        static const color WHITE;
        static const color BLACK;
        static const color GREY;

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

    #define R_G_B_A(color) \
        static_cast<uint8_t>(math::clamp((color).r * 255, 0.0f, 255.0f)), \
        static_cast<uint8_t>(math::clamp((color).g * 255, 0.0f, 255.0f)), \
        static_cast<uint8_t>(math::clamp((color).b * 255, 0.0f, 255.0f)), \
        static_cast<uint8_t>(math::clamp((color).a * 255, 0.0f, 255.0f))                                
}
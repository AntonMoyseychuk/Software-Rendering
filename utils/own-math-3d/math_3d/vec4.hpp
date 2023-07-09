#pragma once
#include "vec2.hpp"
#include "vec3.hpp"

#include <immintrin.h>

namespace math {
    #define R_G_B_A(color) \
        static_cast<uint8_t>(math::clamp((color).r * 255, 0.0f, 255.0f)), \
        static_cast<uint8_t>(math::clamp((color).g * 255, 0.0f, 255.0f)), \
        static_cast<uint8_t>(math::clamp((color).b * 255, 0.0f, 255.0f)), \
        static_cast<uint8_t>(math::clamp((color).a * 255, 0.0f, 255.0f))


    struct vec4f {
        vec4f() noexcept;
        explicit vec4f(float value) noexcept;
        explicit vec4f(const __m128& xmm) noexcept;
        explicit vec4f(const float* arr) noexcept;
        vec4f(float x, float y, float z, float w) noexcept;
        vec4f(const vec4f& vec) noexcept;

        vec4f(const vec2f& xy, const vec2f& zw) noexcept;
        vec4f(const vec2f& xy, float z, float w) noexcept;
        vec4f(float x, const vec2f& yz, float w) noexcept;
        vec4f(float x, float y, const vec2f& zw) noexcept;

        vec4f(const vec3f& xyz, float w) noexcept;
        vec4f(float x, const vec3f& yzw) noexcept;

        vec4f& operator=(const vec2f& vec) noexcept;
        vec4f& operator=(const vec3f& vec) noexcept;
        vec4f& operator=(const vec4f& vec) noexcept;

        vec4f operator-() const noexcept;

        vec4f operator+(const vec4f& vec) const noexcept;
        vec4f operator-(const vec4f& vec) const noexcept;
        vec4f& operator+=(const vec4f& vec) noexcept;
        vec4f& operator-=(const vec4f& vec) noexcept;

        vec4f operator*(const vec4f& vec) const noexcept;
        vec4f& operator*=(const vec4f& vec) noexcept;

        vec4f operator*(float value) const noexcept;
        vec4f& operator*=(float value) noexcept;
        vec4f operator/(float value) const noexcept;
        vec4f& operator/=(float value) noexcept;

        bool operator==(const vec4f& vec) const noexcept;
        bool operator!=(const vec4f& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec4f get_random_vector(float min, float max) noexcept;

        static const vec4f ZERO; 
        static const vec4f LEFT; 
        static const vec4f UP; 
        static const vec4f RIGHT; 
        static const vec4f DOWN; 
        static const vec4f FORWARD; 
        static const vec4f BACKWARD;

        static const vec4f RED;
        static const vec4f GREEN;
        static const vec4f BLUE;
        static const vec4f YELLOW;
        static const vec4f MAGENTA;
        static const vec4f CYAN;
        static const vec4f ORANGE;
        static const vec4f TANGERINE;
        static const vec4f LIME;
        static const vec4f PURPLE;
        static const vec4f INDIGO;
        static const vec4f GOLDEN;
        static const vec4f SKY_BLUE;
        static const vec4f WHITE;
        static const vec4f BLACK;
        static const vec4f GREY;

        union {
            struct {
                float x, y, z, w;
            };

            struct {
                vec2f xy;
                float _ignore_0;
                float _ignore_1;
            };
            
            struct {
                float _ignore_2;
                vec2f yz;
                float _ignore_3;
            };

            struct {
                float _ignore_4;
                float _ignore_5;
                vec2f zw;
            };

            struct {
                vec3f xyz;
                float _ignore_6;
            };

            struct {
                float _ignore_7;
                vec3f yzw;
            };

            struct {
                float r, g, b, a;
            };

            struct {
                vec2f rg;
                float _ignore_8;
                float _ignore_9;
            };
            
            struct {
                float _ignore_10;
                vec2f gb;
                float _ignore_11;
            };

            struct {
                float _ignore_12;
                float _ignore_13;
                vec2f ba;
            };

            struct {
                vec3f rgb;
                float _ignore_14;
            };

            struct {
                float _ignore_15;
                vec3f gba;
            };

            float arr[4];
            
            __m128 xmm;
        };
    };

    using color = vec4f;

    vec4f operator*(float value, const vec4f& vec) noexcept;
    vec4f normalize(const vec4f& vec) noexcept;
}
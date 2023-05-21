#pragma once
#include "vec2.hpp"

namespace math {
    union vec4f;

    union vec3f {
        vec3f() noexcept = default;
        explicit vec3f(float value) noexcept;
        explicit vec3f(const float* arr) noexcept;
        vec3f(float x, float y, float z) noexcept;

        explicit vec3f(const vec2f& vec) noexcept;
        vec3f(const vec2f& xy, float z) noexcept;
        vec3f(float x, const vec2f& yz) noexcept;
        
        explicit vec3f(const vec4f& vec) noexcept;

        vec3f& operator=(const vec2f& vec) noexcept;
        vec3f& operator=(const vec4f& vec) noexcept;

        vec3f operator-() const noexcept;

        vec3f operator+(const vec3f& vec) const noexcept;
        vec3f operator-(const vec3f& vec) const noexcept;
        vec3f& operator+=(const vec3f& vec) noexcept;
        vec3f& operator-=(const vec3f& vec) noexcept;

        vec3f operator*(const vec3f& vec) const noexcept;
        vec3f& operator*=(const vec3f& vec) noexcept;

        vec3f operator*(float value) const noexcept;
        vec3f& operator*=(float value) noexcept;
        vec3f operator/(float value) const noexcept;
        vec3f& operator/=(float value) noexcept;

        bool operator==(const vec3f& vec) const noexcept;
        bool operator!=(const vec3f& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec3f get_random_vector(float min, float max) noexcept;

        static const vec3f ZERO; 
        static const vec3f LEFT; 
        static const vec3f UP; 
        static const vec3f RIGHT; 
        static const vec3f DOWN; 
        static const vec3f FORWARD; 
        static const vec3f BACKWARD; 

        struct {
            float x, y, z;
        };

        struct {
            vec2f xy;
            float _ignore_0;
        };

        struct {
            float _ignore_1;
            vec2f yz;
        };

        float arr[3];
    };

    vec3f operator*(float value, const vec3f& vec) noexcept;

    vec3f normalize(const vec3f& vec) noexcept;
}
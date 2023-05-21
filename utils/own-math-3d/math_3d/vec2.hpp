#pragma once

namespace math {
    union vec3f;
    union vec4f;

    union vec2f {
    public:
        vec2f() noexcept = default;
        explicit vec2f(float value) noexcept;
        explicit vec2f(const float* arr) noexcept;
        vec2f(float x, float y) noexcept;

        explicit vec2f(const vec3f& vec) noexcept;
        explicit vec2f(const vec4f& vec) noexcept;

        vec2f& operator=(const vec3f& vec) noexcept;
        vec2f& operator=(const vec4f& vec) noexcept;

        vec2f operator-() const noexcept;

        vec2f operator+(const vec2f& vec) const noexcept;
        vec2f operator-(const vec2f& vec) const noexcept;
        vec2f& operator+=(const vec2f& vec) noexcept;
        vec2f& operator-=(const vec2f& vec) noexcept;

        vec2f operator*(const vec2f& vec) const noexcept;
        vec2f& operator*=(const vec2f& vec) noexcept;

        vec2f operator*(float value) const noexcept;
        vec2f& operator*=(float value) noexcept;
        vec2f operator/(float value) const noexcept;
        vec2f& operator/=(float value) noexcept;

        bool operator==(const vec2f& vec) const noexcept;
        bool operator!=(const vec2f& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec2f get_random_vector(float min, float max) noexcept;

        static const vec2f ZERO; 
        static const vec2f LEFT; 
        static const vec2f UP; 
        static const vec2f RIGHT; 
        static const vec2f DOWN; 


        struct {
            float x, y;
        };

        float arr[2];
    };

    vec2f operator*(float value, const vec2f& vec) noexcept;

    vec2f normalize(const vec2f& vec) noexcept;
}
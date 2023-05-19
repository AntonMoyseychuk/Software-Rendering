#pragma once

namespace math {
    union vec3;
    union vec4;

    union vec2 {
    public:
        vec2() noexcept = default;
        vec2(float value) noexcept;
        vec2(float x, float y) noexcept;

        vec2(const vec3& vec) noexcept;
        vec2(const vec4& vec) noexcept;

        vec2& operator=(const vec3& vec) noexcept;
        vec2& operator=(const vec4& vec) noexcept;

        vec2 operator-() const noexcept;

        vec2 operator+(const vec2& vec) const noexcept;
        vec2 operator-(const vec2& vec) const noexcept;
        vec2& operator+=(const vec2& vec) noexcept;
        vec2& operator-=(const vec2& vec) noexcept;

        vec2 operator*(const vec2& vec) const noexcept;
        vec2& operator*=(const vec2& vec) noexcept;

        vec2 operator*(float value) const noexcept;
        vec2& operator*=(float value) noexcept;
        vec2 operator/(float value) const noexcept;
        vec2& operator/=(float value) noexcept;

        bool operator==(const vec2& vec) const noexcept;
        bool operator!=(const vec2& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec2 get_random_in_range(float min, float max) noexcept;

        static const vec2 ZERO; 
        static const vec2 LEFT; 
        static const vec2 UP; 
        static const vec2 RIGHT; 
        static const vec2 DOWN; 


        struct {
            float x, y;
        };

        float arr[2];
    };

    vec2 operator*(float value, const vec2& vec) noexcept;

    vec2 normalize(const vec2& vec) noexcept;
}
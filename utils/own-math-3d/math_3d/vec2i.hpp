#pragma once
#include <cstdint>

namespace math {
    union vec2f;
    union vec3f;
    union vec4f;

    union vec2i {
    public:
        vec2i() noexcept = default;
        explicit vec2i(int32_t value) noexcept;
        explicit vec2i(const int32_t* arr) noexcept;
        vec2i(int32_t x, int32_t y) noexcept;

        explicit vec2i(const vec2f& vec) noexcept;
        explicit vec2i(const vec3f& vec) noexcept;
        explicit vec2i(const vec4f& vec) noexcept;

        vec2i& operator=(const vec2f& vec) noexcept;
        vec2i& operator=(const vec3f& vec) noexcept;
        vec2i& operator=(const vec4f& vec) noexcept;

        vec2i operator-() const noexcept;

        vec2i operator+(const vec2i& vec) const noexcept;
        vec2i operator-(const vec2i& vec) const noexcept;
        vec2i& operator+=(const vec2i& vec) noexcept;
        vec2i& operator-=(const vec2i& vec) noexcept;

        vec2i operator*(const vec2i& vec) const noexcept;
        vec2i& operator*=(const vec2i& vec) noexcept;

        vec2i operator*(int32_t value) const noexcept;
        vec2i& operator*=(int32_t value) noexcept;
        vec2i operator/(int32_t value) const noexcept;
        vec2i& operator/=(int32_t value) noexcept;

        bool operator==(const vec2i& vec) const noexcept;
        bool operator!=(const vec2i& vec) const noexcept;

        float length() const noexcept;

        int32_t& operator[](size_t i) noexcept;
        int32_t operator[](size_t i) const noexcept;

        static vec2i get_random_vector(int32_t min, int32_t max) noexcept;

        static const vec2i ZERO; 
        static const vec2i LEFT; 
        static const vec2i UP; 
        static const vec2i RIGHT; 
        static const vec2i DOWN; 


        struct {
            int32_t x, y;
        };

        int32_t arr[2];
    };

    vec2i operator*(int32_t value, const vec2i& vec) noexcept;

    vec2f normalize(const vec2i& vec) noexcept;
}
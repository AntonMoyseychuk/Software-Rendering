#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "math.hpp"

#include <cassert>
#include <cmath>

namespace math {
    const vec2 vec2::LEFT(-1.0f, 0.0f); 
    const vec2 vec2::UP(0.0f, 1.0f); 
    const vec2 vec2::RIGHT(1.0f, 0.0f); 
    const vec2 vec2::DOWN(0.0f, -1.0f); 

    vec2::vec2(float value) noexcept
        : x(value), y(value)
    {
    }
    
    vec2::vec2(float x, float y) noexcept
        : x(x), y(y)
    {
    }
    
    vec2::vec2(const vec3 &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec2::vec2(const vec4 &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec2 &vec2::operator=(const vec3 &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }
    
    vec2 &vec2::operator=(const vec4 &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }

    vec2 vec2::operator-() const noexcept {
        return vec2(-x, -y);
    }

    vec2 vec2::operator+(const vec2 &vec) const noexcept {
        return vec2(x + vec.x, y + vec.y);
    }

    vec2 vec2::operator-(const vec2 &vec) const noexcept {
        return vec2(x - vec.x, y - vec.y);
    }

    vec2 &vec2::operator+=(const vec2 &vec) noexcept {
        x += vec.x;
        y += vec.y;

        return *this;
    }
    
    vec2 &vec2::operator-=(const vec2 &vec) noexcept {
        x -= vec.x;
        y -= vec.y;

        return *this;
    }
    
    vec2 vec2::operator*(float value) const noexcept {
        return vec2(x * value, y * value);
    }
    
    vec2 &vec2::operator*=(float value) noexcept {
        x *= value;
        y *= value;

        return *this;
    }

    vec2 vec2::operator/(float value) const noexcept {
        return vec2(x / value, y / value);
    }

    vec2 &vec2::operator/=(float value) noexcept {
        x /= value;
        y /= value;

        return *this;
    }

    bool vec2::operator==(const vec2 &vec) const noexcept {
        return x == vec.x && y == vec.y;
    }

    bool vec2::operator!=(const vec2 &vec) const noexcept {
        return !(*this == vec);
    }

    float vec2::length() const noexcept {
        return std::sqrtf(x * x + y * y);
    }

    float &vec2::operator[](size_t i) noexcept {
        assert(i < 2);
        return arr[i];
    }

    float vec2::operator[](size_t i) const noexcept {
        assert(i < 2);
        return arr[i];
    }

    vec2 vec2::get_random_in_range(float min, float max) noexcept {
        return vec2(Random(min, max), Random(min, max));
    }

    vec2 operator*(float value, const vec2 &vec) noexcept {
        return vec * value;
    }
    
    vec2 normalize(const vec2 &vec) noexcept {
        return vec * (1.0f / vec.length());
    }
}
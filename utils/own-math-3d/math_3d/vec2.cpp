#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "math.hpp"

#include <cassert>
#include <cmath>

namespace math {
    const vec2f vec2f::ZERO(0.0f); 
    const vec2f vec2f::LEFT(-1.0f, 0.0f); 
    const vec2f vec2f::UP(0.0f, 1.0f); 
    const vec2f vec2f::RIGHT(1.0f, 0.0f); 
    const vec2f vec2f::DOWN(0.0f, -1.0f); 

    vec2f::vec2f(float value) noexcept
        : x(value), y(value)
    {
    }

    vec2f::vec2f(const float *arr) noexcept
        : x(arr[0]), y(arr[1])
    {
    }

    vec2f::vec2f(float x, float y) noexcept
        : x(x), y(y)
    {
    }
    
    vec2f::vec2f(const vec3f &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec2f::vec2f(const vec4f &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec2f &vec2f::operator=(const vec3f &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }
    
    vec2f &vec2f::operator=(const vec4f &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }

    vec2f vec2f::operator-() const noexcept {
        return vec2f(-x, -y);
    }

    vec2f vec2f::operator+(const vec2f &vec) const noexcept {
        return vec2f(x + vec.x, y + vec.y);
    }

    vec2f vec2f::operator-(const vec2f &vec) const noexcept {
        return vec2f(x - vec.x, y - vec.y);
    }

    vec2f &vec2f::operator+=(const vec2f &vec) noexcept {
        x += vec.x;
        y += vec.y;

        return *this;
    }
    
    vec2f &vec2f::operator-=(const vec2f &vec) noexcept {
        x -= vec.x;
        y -= vec.y;

        return *this;
    }

    vec2f vec2f::operator*(const vec2f &vec) const noexcept {
        return vec2f(x * vec.x, y * vec.y);
    }

    vec2f &vec2f::operator*=(const vec2f &vec) noexcept {
        x *= vec.x;
        y *= vec.y;

        return *this;
    }

    vec2f vec2f::operator*(float value) const noexcept {
        return vec2f(x * value, y * value);
    }
    
    vec2f &vec2f::operator*=(float value) noexcept {
        x *= value;
        y *= value;

        return *this;
    }

    vec2f vec2f::operator/(float value) const noexcept {
        return vec2f(x / value, y / value);
    }

    vec2f &vec2f::operator/=(float value) noexcept {
        x /= value;
        y /= value;

        return *this;
    }

    bool vec2f::operator==(const vec2f &vec) const noexcept {
        return is_tends_to(x, vec.x) && is_tends_to(y, vec.y);
    }

    bool vec2f::operator!=(const vec2f &vec) const noexcept {
        return !(*this == vec);
    }

    float vec2f::length() const noexcept {
        return std::sqrtf(x * x + y * y);
    }

    float &vec2f::operator[](size_t i) noexcept {
        assert(i < 2);
        return arr[i];
    }

    float vec2f::operator[](size_t i) const noexcept {
        assert(i < 2);
        return arr[i];
    }

    vec2f vec2f::get_random_vector(float min, float max) noexcept {
        return vec2f(random(min, max), random(min, max));
    }

    vec2f operator*(float value, const vec2f &vec) noexcept {
        return vec * value;
    }
    
    vec2f normalize(const vec2f &vec) noexcept {
        return vec * (1.0f / vec.length());
    }
}
#include "vec2i.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "util.hpp"

#include <cassert>
#include <cmath>

namespace math {
    const vec2i vec2i::ZERO(0); 
    const vec2i vec2i::LEFT(-1, 0); 
    const vec2i vec2i::UP(0, 1); 
    const vec2i vec2i::RIGHT(1, 0); 
    const vec2i vec2i::DOWN(0, -1); 

    vec2i::vec2i(int32_t value) noexcept
        : x(value), y(value)
    {
    }

    vec2i::vec2i(const int32_t *arr) noexcept
        : x(arr[0]), y(arr[1])
    {
    }

    vec2i::vec2i(int32_t x, int32_t y) noexcept
        : x(x), y(y)
    {
    }

    vec2i::vec2i(const vec2f &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }

    vec2i::vec2i(const vec3f &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec2i::vec2i(const vec4f &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }

    vec2i &vec2i::operator=(const vec2f &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }

    vec2i &vec2i::operator=(const vec3f &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }

    vec2i &vec2i::operator=(const vec4f &vec) noexcept {
        x = vec.x;
        y = vec.y;

        return *this;
    }

    vec2i vec2i::operator-() const noexcept {
        return vec2i(-x, -y);
    }

    vec2i vec2i::operator+(const vec2i &vec) const noexcept {
        return vec2i(x + vec.x, y + vec.y);
    }

    vec2i vec2i::operator-(const vec2i &vec) const noexcept {
        return vec2i(x - vec.x, y - vec.y);
    }

    vec2i &vec2i::operator+=(const vec2i &vec) noexcept {
        x += vec.x;
        y += vec.y;

        return *this;
    }
    
    vec2i &vec2i::operator-=(const vec2i &vec) noexcept {
        x -= vec.x;
        y -= vec.y;

        return *this;
    }

    vec2i vec2i::operator*(const vec2i &vec) const noexcept {
        return vec2i(x * vec.x, y * vec.y);
    }

    vec2i &vec2i::operator*=(const vec2i &vec) noexcept {
        x *= vec.x;
        y *= vec.y;

        return *this;
    }

    vec2i vec2i::operator*(int32_t value) const noexcept {
        return vec2i(x * value, y * value);
    }
    
    vec2i &vec2i::operator*=(int32_t value) noexcept {
        x *= value;
        y *= value;

        return *this;
    }

    vec2i vec2i::operator/(int32_t value) const noexcept {
        assert(value != 0);
        return vec2i(x / value, y / value);
    }

    vec2i &vec2i::operator/=(int32_t value) noexcept {
        assert(value != 0);
        x /= value;
        y /= value;

        return *this;
    }

    bool vec2i::operator==(const vec2i &vec) const noexcept {
        return x == vec.x && y == vec.y;
    }

    bool vec2i::operator!=(const vec2i &vec) const noexcept {
        return !(*this == vec);
    }

    float vec2i::length() const noexcept {
        return std::sqrtf(x * x + y * y);
    }

    int32_t &vec2i::operator[](size_t i) noexcept {
        assert(i < 2);
        return arr[i];
    }

    int32_t vec2i::operator[](size_t i) const noexcept {
        assert(i < 2);
        return arr[i];
    }

    vec2i vec2i::get_random_vector(int32_t min, int32_t max) noexcept {
        return vec2i(random(min, max), random(min, max));
    }

    vec2i operator*(int32_t value, const vec2i &vec) noexcept {
        return vec * value;
    }
    
    vec2f normalize(const vec2i &vec) noexcept {
        const float length = vec.length();
        assert(!is_tends_to(length, 0.0f));
        return vec2f(vec.x / length, vec.y / length);
    }
}
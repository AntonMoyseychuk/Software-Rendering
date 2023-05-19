#include "vec3.hpp"
#include "vec4.hpp"

#include "math.hpp"

#include <cassert>
#include <cmath>

namespace math {
    const vec3 vec3::ZERO(0.0f); 
    const vec3 vec3::LEFT(-1.0f, 0.0f, 0.0f); 
    const vec3 vec3::UP(0.0f, 1.0f, 0.0f); 
    const vec3 vec3::RIGHT(1.0f, 0.0f, 0.0f); 
    const vec3 vec3::DOWN(0.0f, -1.0f, 0.0f); 
    const vec3 vec3::FORWARD(0.0f, 0.0f, 1.0f); 
    const vec3 vec3::BACKWARD(0.0f, 0.0f, -1.0f);


    vec3::vec3(float value) noexcept
        : x(value), y(value), z(value)
    {
    }

    vec3::vec3(const float *arr) noexcept
        : x(arr[0]), y(arr[1]), z(arr[2])
    {
    }

    vec3::vec3(float x, float y, float z) noexcept
        : x(x), y(y), z(z)
    {
    }
    
    vec3::vec3(const vec2 &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec3::vec3(const vec2 &xy, float z) noexcept
        : x(xy.x), y(xy.y), z(z)
    {
    }
    
    vec3::vec3(float x, const vec2 &yz) noexcept
        : x(x), y(yz.x), z(yz.y)
    {
    }

    vec3::vec3(const vec4 &vec) noexcept
        : x(vec.x), y(vec.y), z(vec.z)
    {
    }
    
    vec3 &vec3::operator=(const vec2 &vec) noexcept {
        x = vec.x;
        y = vec.y;
        z = 0.0f;

        return *this;
    }

    vec3 &vec3::operator=(const vec4 &vec) noexcept {
        x = vec.x;
        y = vec.y;
        z = vec.z;

        return *this;
    }

    vec3 vec3::operator-() const noexcept {
        return vec3(-x, -y, -z);
    }

    vec3 vec3::operator+(const vec3 &vec) const noexcept {
        return vec3(x + vec.x, y + vec.y, z + vec.z);
    }

    vec3 vec3::operator-(const vec3 &vec) const noexcept {
        return vec3(x - vec.x, y - vec.y, z - vec.z);
    }

    vec3 &vec3::operator+=(const vec3 &vec) noexcept {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    vec3 &vec3::operator-=(const vec3 &vec) noexcept {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    vec3 vec3::operator*(float value) const noexcept {
        return vec3(x * value, y * value, z * value);
    }

    vec3 &vec3::operator*=(float value) noexcept {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    vec3 vec3::operator/(float value) const noexcept {
        return vec3(x / value, y / value, z / value);
    }

    vec3 &vec3::operator/=(float value) noexcept {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    bool vec3::operator==(const vec3 &vec) const noexcept {
        return x == vec.x && y == vec.y && z == vec.z;
    }

    bool vec3::operator!=(const vec3 &vec) const noexcept {
        return !(*this == vec);
    }

    float vec3::length() const noexcept {
        return std::sqrtf(x * x + y * y + z * z);
    }

    float &vec3::operator[](size_t i) noexcept {
        assert(i < 3);
        return arr[i];
    }

    float vec3::operator[](size_t i) const noexcept {
        assert(i < 3);
        return arr[i];
    }

    vec3 vec3::get_random_in_range(float min, float max) noexcept {
        return vec3(Random(min, max), Random(min, max), Random(min, max));
    }

    vec3 operator*(float value, const vec3 &vec) noexcept {
        return vec * value;
    }

    vec3 normalize(const vec3 &vec) noexcept {
        return vec * (1.0f / vec.length());
    }
}
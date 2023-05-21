#include "vec3.hpp"
#include "vec4.hpp"

#include "util.hpp"

#include <cassert>
#include <cmath>

namespace math {
    const vec3f vec3f::ZERO(0.0f); 
    const vec3f vec3f::LEFT(-1.0f, 0.0f, 0.0f); 
    const vec3f vec3f::UP(0.0f, 1.0f, 0.0f); 
    const vec3f vec3f::RIGHT(1.0f, 0.0f, 0.0f); 
    const vec3f vec3f::DOWN(0.0f, -1.0f, 0.0f); 
    const vec3f vec3f::FORWARD(0.0f, 0.0f, 1.0f); 
    const vec3f vec3f::BACKWARD(0.0f, 0.0f, -1.0f);


    vec3f::vec3f(float value) noexcept
        : x(value), y(value), z(value)
    {
    }

    vec3f::vec3f(const float *arr) noexcept
        : x(arr[0]), y(arr[1]), z(arr[2])
    {
    }

    vec3f::vec3f(float x, float y, float z) noexcept
        : x(x), y(y), z(z)
    {
    }

    vec3f::vec3f(const vec2f &vec) noexcept
        : x(vec.x), y(vec.y)
    {
    }
    
    vec3f::vec3f(const vec2f &xy, float z) noexcept
        : x(xy.x), y(xy.y), z(z)
    {
    }
    
    vec3f::vec3f(float x, const vec2f &yz) noexcept
        : x(x), y(yz.x), z(yz.y)
    {
    }

    vec3f::vec3f(const vec4f &vec) noexcept
        : x(vec.x), y(vec.y), z(vec.z)
    {
    }
    
    vec3f &vec3f::operator=(const vec2f &vec) noexcept {
        x = vec.x;
        y = vec.y;
        z = 0.0f;

        return *this;
    }

    vec3f &vec3f::operator=(const vec4f &vec) noexcept {
        x = vec.x;
        y = vec.y;
        z = vec.z;

        return *this;
    }

    vec3f vec3f::operator-() const noexcept {
        return vec3f(-x, -y, -z);
    }

    vec3f vec3f::operator+(const vec3f &vec) const noexcept {
        return vec3f(x + vec.x, y + vec.y, z + vec.z);
    }

    vec3f vec3f::operator-(const vec3f &vec) const noexcept {
        return vec3f(x - vec.x, y - vec.y, z - vec.z);
    }

    vec3f &vec3f::operator+=(const vec3f &vec) noexcept {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    vec3f &vec3f::operator-=(const vec3f &vec) noexcept {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    vec3f vec3f::operator*(const vec3f &vec) const noexcept {
        return vec3f(x * vec.x, y * vec.y, z * vec.z);
    }

    vec3f &vec3f::operator*=(const vec3f &vec) noexcept {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;

        return *this;
    }

    vec3f vec3f::operator*(float value) const noexcept {
        return vec3f(x * value, y * value, z * value);
    }

    vec3f &vec3f::operator*=(float value) noexcept {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    vec3f vec3f::operator/(float value) const noexcept {
        return vec3f(x / value, y / value, z / value);
    }

    vec3f &vec3f::operator/=(float value) noexcept {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    bool vec3f::operator==(const vec3f &vec) const noexcept {
        return is_tends_to(x, vec.x) && is_tends_to(y, vec.y) && is_tends_to(z, vec.z);
    }

    bool vec3f::operator!=(const vec3f &vec) const noexcept {
        return !(*this == vec);
    }

    float vec3f::length() const noexcept {
        return std::sqrtf(x * x + y * y + z * z);
    }

    float &vec3f::operator[](size_t i) noexcept {
        assert(i < 3);
        return arr[i];
    }

    float vec3f::operator[](size_t i) const noexcept {
        assert(i < 3);
        return arr[i];
    }

    vec3f vec3f::get_random_vector(float min, float max) noexcept {
        return vec3f(random(min, max), random(min, max), random(min, max));
    }

    vec3f operator*(float value, const vec3f &vec) noexcept {
        return vec * value;
    }

    vec3f normalize(const vec3f &vec) noexcept {
        return vec * (1.0f / vec.length());
    }
}
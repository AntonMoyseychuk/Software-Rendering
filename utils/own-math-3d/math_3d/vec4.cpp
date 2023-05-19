#include "vec4.hpp"

#include "math.hpp"

#include <cmath>
#include <cassert>

namespace math {
    const vec4 vec4::LEFT(-1.0f, 0.0f, 0.0f, 0.0f); 
    const vec4 vec4::UP(0.0f, 1.0f, 0.0f, 0.0f); 
    const vec4 vec4::RIGHT(1.0f, 0.0f, 0.0f, 0.0f); 
    const vec4 vec4::DOWN(0.0f, -1.0f, 0.0f, 0.0f); 
    const vec4 vec4::FORWARD(0.0f, 0.0f, 1.0f, 0.0f); 
    const vec4 vec4::BACKWARD(0.0f, 0.0f, -1.0f, 0.0f);

    const color color::RED(1.0f, 0.0f, 0.0f, 1.0f);
    const color color::GREEN(0.0f, 0.5f, 0.0f, 1.0f);
    const color color::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
    const color color::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
    const color color::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
    const color color::CYAN(0.0f, 1.0f, 1.0f, 0.0f);
    const color color::ORANGE(1.0f, 0.4f, 0.0, 1.0f);
    const color color::LIME(0.0f, 1.0f, 0.0f, 1.0f);
    const color color::PURPLE(0.5f, 0.0f, 0.5f, 1.0f);
    const color color::INDIGO(75.0f / 255.0f, 0.0f, 130.0f / 255.0f, 1.0f);
    const color color::GOLDEN(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f, 1.0f);
    const color color::SKY_BLUE(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    const color color::WHITE(1.0f);
    const color color::BLACK(0.0f, 0.0f, 0.0f, 0.0f);
    const color color::GREY(0.5f, 0.5f, 0.5f, 1.0f);


    vec4::vec4(float value) noexcept
        : mm_128(_mm_set_ps1(value))
    {
    }

    vec4::vec4(const __m128 &mm_128) noexcept
        : mm_128(mm_128)
    {
    }

    vec4::vec4(float x, float y, float z, float w) noexcept
        : mm_128(_mm_set_ps(w, z, y, x))
    {
    }
    
    vec4::vec4(const vec2 &vec) noexcept
        : mm_128(_mm_set_ps(0.0f, 0.0f, vec.y, vec.x)) 
    {
    }

    vec4::vec4(const vec2 &xy, const vec2 &zw) noexcept
        : mm_128(_mm_set_ps(zw.y, zw.x, xy.y, xy.x))
    {
    }

    vec4::vec4(const vec2 &xy, float z, float w) noexcept
        : mm_128(_mm_set_ps(w, z, xy.y, xy.x)) 
    {
    }
    
    vec4::vec4(float x, const vec2 &yz, float w) noexcept
        : mm_128(_mm_set_ps(w, yz.y, yz.x, x))
    {
    }
    
    vec4::vec4(float x, float y, const vec2 &zw) noexcept
        : mm_128(_mm_set_ps(zw.y, zw.x, y, x))
    {
    }
    
    vec4::vec4(const vec3 &vec) noexcept
        : mm_128(_mm_set_ps(0.0f, vec.z, vec.y, vec.x))
    {
    }
    
    vec4::vec4(const vec3 &xyz, float w) noexcept
        : mm_128(_mm_set_ps(w, xyz.z, xyz.y, xyz.x))
    {
    }
    
    vec4::vec4(float x, const vec3 &yzw) noexcept
        : mm_128(_mm_set_ps(xyz.z, xyz.y, xyz.x, x))
    {
    }

    vec4 &vec4::operator=(const vec2 &vec) noexcept {
        mm_128 = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);

        return *this;
    }

    vec4 &vec4::operator=(const vec3 &vec) noexcept {
        mm_128 = _mm_set_ps(0.0f, vec.z, vec.y, vec.x);

        return *this;
    }

    vec4 vec4::operator-() const noexcept {
        return vec4(-x, -y, -z, -w);
    }

    vec4 vec4::operator+(const vec4 &vec) const noexcept {
        return vec4(_mm_add_ps(mm_128, vec.mm_128));
    }

    vec4 vec4::operator-(const vec4 &vec) const noexcept {
        return vec4(_mm_sub_ps(mm_128, vec.mm_128));
    }

    vec4 &vec4::operator+=(const vec4 &vec) noexcept {
        mm_128 = _mm_add_ps(mm_128, vec.mm_128);
        return *this;
    }

    vec4 &vec4::operator-=(const vec4 &vec) noexcept {
        mm_128 = _mm_sub_ps(mm_128, vec.mm_128);
        return *this;
    }

    vec4 vec4::operator*(float value) const noexcept {
        return vec4(_mm_mul_ps(mm_128, _mm_set_ps1(value)));
    }

    vec4 &vec4::operator*=(float value) noexcept {
        mm_128 = _mm_mul_ps(mm_128, _mm_set_ps1(value));
        return *this;
    }

    vec4 vec4::operator/(float value) const noexcept {
        return vec4(_mm_div_ps(mm_128, _mm_set_ps1(value)));
    }

    vec4 &vec4::operator/=(float value) noexcept{
        mm_128 = _mm_div_ps(mm_128, _mm_set_ps1(value));
        return *this;
    }

    bool vec4::operator==(const vec4 &vec) const noexcept {
        return _mm_cvtss_f32(_mm_cmpeq_ps(mm_128, vec.mm_128));
    }

    bool vec4::operator!=(const vec4 &vec) const noexcept {
        return !(*this == vec);
    }

    float vec4::length() const noexcept {
        return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(mm_128, mm_128, 0xF1)));
    }

    float &vec4::operator[](size_t i) noexcept {
        assert(i < 4);
        return arr[i];
    }

    float vec4::operator[](size_t i) const noexcept {
        assert(i < 4);
        return arr[i];
    }

    vec4 vec4::get_random_in_range(float min, float max) noexcept {
        return vec4(Random(min, max), Random(min, max), Random(min, max), Random(min, max));
    }

    vec4 operator*(float value, const vec4 &vec) noexcept {
        return vec * value;
    }

    vec4 normalize(const vec4 &vec) noexcept {
        const __m128 div = _mm_set_ps1(vec.length());
        return vec4(_mm_div_ps(vec.mm_128, div));
    }
}
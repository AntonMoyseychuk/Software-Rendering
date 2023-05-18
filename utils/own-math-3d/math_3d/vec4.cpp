#include "vec4.hpp"
#include "quaternion.hpp"

#include "math.hpp"

#include <cmath>
#include <cassert>

namespace math {
    const vec4 vec4::left(-1.0f, 0.0f, 0.0f, 0.0f); 
    const vec4 vec4::up(0.0f, 1.0f, 0.0f, 0.0f); 
    const vec4 vec4::right(1.0f, 0.0f, 0.0f, 0.0f); 
    const vec4 vec4::down(0.0f, -1.0f, 0.0f, 0.0f); 
    const vec4 vec4::forward(0.0f, 0.0f, 1.0f, 0.0f); 
    const vec4 vec4::backward(0.0f, 0.0f, -1.0f, 0.0f);

    const color color::red(1.0f, 0.0f, 0.0f, 1.0f);
    const color color::green(0.0f, 0.5f, 0.0f, 1.0f);
    const color color::blue(0.0f, 0.0f, 1.0f, 1.0f);
    const color color::yellow(1.0f, 1.0f, 0.0f, 1.0f);
    const color color::magenta(1.0f, 0.0f, 1.0f, 1.0f);
    const color color::cyan(0.0f, 1.0f, 1.0f, 0.0f);
    const color color::orange(1.0f, 0.4f, 0.0, 1.0f);
    const color color::lime(0.0f, 1.0f, 0.0f, 1.0f);
    const color color::purple(0.5f, 0.0f, 0.5f, 1.0f);
    const color color::indigo(75.0f / 255.0f, 0.0f, 130.0f / 255.0f, 1.0f);
    const color color::golden(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f, 1.0f);
    const color color::sky_blue(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    const color color::white(1.0f);
    const color color::black(0.0f, 0.0f, 0.0f, 1.0f);
    const color color::grey(0.5f, 0.5f, 0.5f, 1.0f);


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

    vec4 vec4::operator*(const quaternion &q) const noexcept {
        const auto rotated = q * quaternion(0, x, y, z) * conjugate(q);
        return vec4(rotated.x, rotated.y, rotated.z, 0.0f);
    }

    vec4 &vec4::operator*=(const quaternion &q) noexcept {
        *this = *this * q;

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
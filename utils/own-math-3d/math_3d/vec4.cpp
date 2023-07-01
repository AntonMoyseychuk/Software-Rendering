#include "vec4.hpp"

#include "util.hpp"

#include <cmath>
#include <cassert>

namespace math {
    const vec4f vec4f::ZERO(0.0f); 
    const vec4f vec4f::LEFT(-1.0f, 0.0f, 0.0f, 0.0f); 
    const vec4f vec4f::UP(0.0f, 1.0f, 0.0f, 0.0f); 
    const vec4f vec4f::RIGHT(1.0f, 0.0f, 0.0f, 0.0f); 
    const vec4f vec4f::DOWN(0.0f, -1.0f, 0.0f, 0.0f); 
    const vec4f vec4f::FORWARD(0.0f, 0.0f, 1.0f, 0.0f); 
    const vec4f vec4f::BACKWARD(0.0f, 0.0f, -1.0f, 0.0f);

    const color color::RED(1.0f, 0.0f, 0.0f, 1.0f);
    const color color::GREEN(0.0f, 0.5f, 0.0f, 1.0f);
    const color color::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
    const color color::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
    const color color::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
    const color color::CYAN(0.0f, 1.0f, 1.0f, 0.0f);
    const color color::ORANGE(1.0f, 0.4f, 0.0, 1.0f);
    const color color::TANGERINE(1.0f, 155.0f / 255.0f, 0.0, 1.0f);
    const color color::LIME(0.0f, 1.0f, 0.0f, 1.0f);
    const color color::PURPLE(0.5f, 0.0f, 0.5f, 1.0f);
    const color color::INDIGO(75.0f / 255.0f, 0.0f, 130.0f / 255.0f, 1.0f);
    const color color::GOLDEN(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f, 1.0f);
    const color color::SKY_BLUE(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    const color color::WHITE(1.0f);
    const color color::BLACK(0.0f, 0.0f, 0.0f, 0.0f);
    const color color::GREY(0.5f, 0.5f, 0.5f, 1.0f);

    vec4f::vec4f() noexcept
        : xmm(_mm_set_ps1(0.0f))
    {
    }

    vec4f::vec4f(float value) noexcept
        : xmm(_mm_set_ps1(value))
    {
    }

    vec4f::vec4f(const __m128 &xmm) noexcept
        : xmm(xmm)
    {
    }

    vec4f::vec4f(const float *arr) noexcept
        : xmm(_mm_load_ps(arr))
    {
    }

    vec4f::vec4f(float x, float y, float z, float w) noexcept
        : xmm(_mm_set_ps(w, z, y, x))
    {
    }

    vec4f::vec4f(const vec4f &vec) noexcept
        : xmm(vec.xmm)
    {
    }

    vec4f::vec4f(const vec2f &xy, const vec2f &zw) noexcept
        : xmm(_mm_set_ps(zw.y, zw.x, xy.y, xy.x))
    {
    }

    vec4f::vec4f(const vec2f &xy, float z, float w) noexcept
        : xmm(_mm_set_ps(w, z, xy.y, xy.x)) 
    {
    }
    
    vec4f::vec4f(float x, const vec2f &yz, float w) noexcept
        : xmm(_mm_set_ps(w, yz.y, yz.x, x))
    {
    }
    
    vec4f::vec4f(float x, float y, const vec2f &zw) noexcept
        : xmm(_mm_set_ps(zw.y, zw.x, y, x))
    {
    }
    
    vec4f::vec4f(const vec3f &xyz, float w) noexcept
        : xmm(_mm_set_ps(w, xyz.z, xyz.y, xyz.x))
    {
    }
    
    vec4f::vec4f(float x, const vec3f &yzw) noexcept
        : xmm(_mm_set_ps(xyz.z, xyz.y, xyz.x, x))
    {
    }

    vec4f &vec4f::operator=(const vec2f &vec) noexcept {
        xmm = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);

        return *this;
    }

    vec4f &vec4f::operator=(const vec3f &vec) noexcept {
        xmm = _mm_set_ps(0.0f, vec.z, vec.y, vec.x);

        return *this;
    }

    vec4f &vec4f::operator=(const vec4f &vec) noexcept {
        xmm = vec.xmm;

        return *this;
    }

    vec4f vec4f::operator-() const noexcept {
        return vec4f(-x, -y, -z, -w);
    }

    vec4f vec4f::operator+(const vec4f &vec) const noexcept {
        return vec4f(_mm_add_ps(xmm, vec.xmm));
    }

    vec4f vec4f::operator-(const vec4f &vec) const noexcept {
        return vec4f(_mm_sub_ps(xmm, vec.xmm));
    }

    vec4f &vec4f::operator+=(const vec4f &vec) noexcept {
        xmm = _mm_add_ps(xmm, vec.xmm);
        return *this;
    }

    vec4f &vec4f::operator-=(const vec4f &vec) noexcept {
        xmm = _mm_sub_ps(xmm, vec.xmm);
        return *this;
    }

    vec4f vec4f::operator*(const vec4f &vec) const noexcept {
        return vec4f(_mm_mul_ps(xmm, vec.xmm));
    }

    vec4f &vec4f::operator*=(const vec4f &vec) noexcept {
        xmm = _mm_mul_ps(xmm, vec.xmm);
        return *this;
    }

    vec4f vec4f::operator*(float value) const noexcept {
        return vec4f(_mm_mul_ps(xmm, _mm_set_ps1(value)));
    }

    vec4f &vec4f::operator*=(float value) noexcept {
        xmm = _mm_mul_ps(xmm, _mm_set_ps1(value));
        return *this;
    }

    vec4f vec4f::operator/(float value) const noexcept {
        return vec4f(_mm_div_ps(xmm, _mm_set_ps1(value)));
    }

    vec4f &vec4f::operator/=(float value) noexcept{
        xmm = _mm_div_ps(xmm, _mm_set_ps1(value));
        return *this;
    }

    bool vec4f::operator==(const vec4f &vec) const noexcept {
        return _mm_movemask_ps(_mm_cmpeq_ps(xmm, vec.xmm)) == 0xF;
    }

    bool vec4f::operator!=(const vec4f &vec) const noexcept {
        return !(*this == vec);
    }

    float vec4f::length() const noexcept {
        return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(xmm, xmm, 0xF1)));
    }

    float &vec4f::operator[](size_t i) noexcept {
        assert(i < 4);
        return arr[i];
    }

    float vec4f::operator[](size_t i) const noexcept {
        assert(i < 4);
        return arr[i];
    }

    vec4f vec4f::get_random_vector(float min, float max) noexcept {
        return vec4f(random(min, max), random(min, max), random(min, max), random(min, max));
    }

    vec4f operator*(float value, const vec4f &vec) noexcept {
        return vec * value;
    }

    vec4f normalize(const vec4f &vec) noexcept {
        const __m128 div = _mm_set_ps1(vec.length());
        return vec4f(_mm_div_ps(vec.xmm, div));
    }
}
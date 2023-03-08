#pragma once
#include <type_traits>
#include <cassert>

namespace math {
    enum class VectorComponent : std::size_t {
        x, y, z, w
    };

    template <typename _Type>
    struct Vector {
        static_assert(std::is_arithmetic_v<_Type>, "_Type must be an arithmetic type");
    
#pragma region interface
        Vector() : Vector(0.0f) 
        {
        }
        Vector(_Type _x, _Type _y, _Type _z, _Type _w = _Type(0))
            : x(_x), y(_y), z(_z), w(_w) 
        {
        }
        Vector(_Type value)
            : x(value), y(value), z(value), w(value) 
        {
        }

        float Length() const noexcept {
            return sqrtf(x * x + y * y + z * z + w * w);
        }
        Vector Normalize() const noexcept {
            auto length = this->Length();

            if (length <= MATH_EPSILON) {
                return *this;
            }

            return Vector(x / length, y / length, z / length, w / length);
        }

        Vector operator+(const Vector &vec) const noexcept {
            return Vector(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
        }

        Vector &operator+=(const Vector &vec) noexcept {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            w += vec.w;
            return *this;
        }

        Vector operator-(const Vector &vec) const noexcept {
            return Vector(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
        }

        Vector &operator-=(const Vector &vec) noexcept {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            w -= vec.w;
            return *this;
        }

        Vector operator*(float value) const noexcept {
            return Vector(x * value, y * value, z * value, w * value);
        }

        Vector& operator*=(float value) noexcept {
            x *= value;
            y *= value;
            z *= value;
            w *= value;
            return *this;
        }

        Vector operator/(float value) const noexcept {
            return Vector(x / value, y / value, z / value, w / value);
        }

        Vector& operator/=(float value) noexcept {
            x /= value;
            y /= value;
            z /= value;
            w /= value;
            return *this;
        }

        Vector operator-() const {
            return Vector(-x, -y, -z, -w);
        }

        _Type& operator[](std::size_t idx) {
            assert(idx < 4);
            return *(&x + idx);
        }
        const _Type& operator[](std::size_t idx) const {
            assert(idx < 4);
            return *(&x + idx);
        }
        _Type& operator[](VectorComponent component) {
            return this->operator[](static_cast<std::size_t>(component));    
        }
        const _Type& operator[](VectorComponent component) const {
            return this->operator[](static_cast<std::size_t>(component));
        }

#pragma endregion interface

#pragma region components 
        _Type x, y, z, w;
#pragma endregion components
    };

#pragma region consts
        static const Vector<float> VECTOR_RIGHT(1.0f, 0.0f, 0.0f);
        static const Vector<float> VECTOR_DOWN(0.0f, -1.0f, 0.0f);
        static const Vector<float> VECTOR_LEFT(-1.0f, 0.0f, 0.0f);
        static const Vector<float> VECTOR_UP(0.0f, 1.0f, 0.0f);
        static const Vector<float> VECTOR_FORWARD(0.0f, 0.0f, 1.0f);
        static const Vector<float> VECTOR_BACKWARD(0.0f, 0.0f, -1.0f);
#pragma endregion consts

    using vec4f = Vector<float>;
    using vec4d = Vector<double>;
    using vec4b = Vector<std::int8_t>;
    using vec4ub = Vector<std::uint8_t>;
    using vec4i = Vector<std::int32_t>;
    using vec4ui = Vector<std::uint32_t>;
}
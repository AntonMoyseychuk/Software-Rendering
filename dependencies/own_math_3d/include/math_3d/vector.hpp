#pragma once
#include <type_traits>
#include <cassert>

namespace math {
    template <typename Type, std::size_t Dimension>
    struct Vector {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type");
        static_assert(Dimension >= 2 && Dimension <= 4, "Dimension must be between 2 and 4");
    };

    template <typename Type>
    struct Vector<Type, 4> {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type");
    
    #pragma region interface
        Vector() : Vector(0.0f) 
        {
        }
        Vector(Type _x, Type _y, Type _z, Type _w = Type(0)) : x(_x), y(_y), z(_z), w(_w) 
        {
        }
        Vector(Type value) : x(value), y(value), z(value), w(value) 
        {
        }

        float Length() const noexcept {
            return sqrtf(x * x + y * y + z * z + w * w);
        }

        Vector Normalize() const noexcept {
            auto length = this->Length();

            if (LinMath::IsTendsTo(length, MATH_EPSILON)) {
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

        friend Vector operator*(float value, const Vector<Type, 4>& vector) noexcept {
            return vector * value;   
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

        Type& operator[](std::size_t idx) {
            assert(idx < 4);
            return *(&x + idx);
        }
        const Type& operator[](std::size_t idx) const {
            assert(idx < 4);
            return *(&x + idx);
        }
    #pragma endregion interface

        Type x, y, z, w;
    };

    template <typename Type>
    struct Vector<Type, 3> {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic value");
    
    #pragma region interface
        Vector() : Vector(0.0f) 
        {
        }
        Vector(Type _x, Type _y, Type _z) : x(_x), y(_y), z(_z)
        {
        }
        Vector(Type value) : x(value), y(value), z(value) 
        {
        }

        float Length() const noexcept {
            return sqrtf(x * x + y * y + z * z);
        }

        Vector Normalize() const noexcept {
            auto length = this->Length();

            if (IsTendsTo(length, MATH_EPSILON)) {
                return *this;
            }

            return Vector(x / length, y / length, z / length);
        }

        Vector operator+(const Vector &vec) const noexcept {
            return Vector(x + vec.x, y + vec.y, z + vec.z);
        }

        Vector &operator+=(const Vector &vec) noexcept {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            return *this;
        }

        Vector operator-(const Vector &vec) const noexcept {
            return Vector(x - vec.x, y - vec.y, z - vec.z);
        }

        Vector &operator-=(const Vector &vec) noexcept {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            return *this;
        }

        Vector operator*(float value) const noexcept {
            return Vector(x * value, y * value, z * value);
        }

        friend Vector operator*(float value, const Vector<Type, 3>& vector) noexcept {
            return vector * value;   
        }

        Vector& operator*=(float value) noexcept {
            x *= value;
            y *= value;
            z *= value;
            return *this;
        }

        Vector operator/(float value) const noexcept {
            return Vector(x / value, y / value, z / value);
        }

        Vector& operator/=(float value) noexcept {
            x /= value;
            y /= value;
            z /= value;
            return *this;
        }

        Vector operator-() const {
            return Vector(-x, -y, -z);
        }

        Type& operator[](std::size_t idx) {
            assert(idx < 3);
            return *(&x + idx);
        }
        const Type& operator[](std::size_t idx) const {
            assert(idx < 3);
            return *(&x + idx);
        }
    #pragma endregion interface

        Type x, y, z;
    };

        template <typename Type>
    struct Vector<Type, 2> {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type");
    
    #pragma region interface
        Vector() : Vector(0.0f) 
        {
        }
        Vector(Type _x, Type _y) : x(_x), y(_y)
        {
        }
        Vector(Type value) : x(value), y(value) 
        {
        }

        float Length() const noexcept {
            return sqrtf(x * x + y * y);
        }

        Vector Normalize() const noexcept {
            auto length = this->Length();

            if (LinMath::IsTendsTo(length, MATH_EPSILON)) {
                return *this;
            }

            return Vector(x / length, y / length);
        }

        Vector operator+(const Vector &vec) const noexcept {
            return Vector(x + vec.x, y + vec.y);
        }

        Vector &operator+=(const Vector &vec) noexcept {
            x += vec.x;
            y += vec.y;
            return *this;
        }

        Vector operator-(const Vector &vec) const noexcept {
            return Vector(x - vec.x, y - vec.y);
        }

        Vector &operator-=(const Vector &vec) noexcept {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }

        Vector operator*(float value) const noexcept {
            return Vector(x * value, y * value);
        }

        friend Vector operator*(float value, const Vector<Type, 2>& vector) noexcept {
            return vector * value;   
        }

        Vector& operator*=(float value) noexcept {
            x *= value;
            y *= value;
            return *this;
        }

        Vector operator/(float value) const noexcept {
            return Vector(x / value, y / value);
        }

        Vector& operator/=(float value) noexcept {
            x /= value;
            y /= value;
            return *this;
        }

        Vector operator-() const {
            return Vector(-x, -y);
        }

        Type& operator[](std::size_t idx) {
            assert(idx < 2);
            return *(&x + idx);
        }
        const Type& operator[](std::size_t idx) const {
            assert(idx < 2);
            return *(&x + idx);
        }
    #pragma endregion interface

        Type x, y;
    };

    using vec4f = Vector<float, 4>;
    using vec4d = Vector<double, 4>;
    using vec4b = Vector<std::int8_t, 4>;
    using vec4ub = Vector<std::uint8_t, 4>;
    using vec4i = Vector<std::int32_t, 4>;
    using vec4ui = Vector<std::uint32_t, 4>;

    using vec3f = Vector<float, 3>;
    using vec3d = Vector<double, 3>;
    using vec3b = Vector<std::int8_t, 3>;
    using vec3ub = Vector<std::uint8_t, 3>;
    using vec3i = Vector<std::int32_t, 3>;
    using vec3ui = Vector<std::uint32_t, 3>;

    using vec2f = Vector<float, 2>;
    using vec2d = Vector<double, 2>;
    using vec2b = Vector<std::int8_t, 2>;
    using vec2ub = Vector<std::uint8_t, 2>;
    using vec2i = Vector<std::int32_t, 2>;
    using vec2ui = Vector<std::uint32_t, 2>;

    static const vec3f VECTOR_RIGHT(1.0f, 0.0f, 0.0f);
    static const vec3f VECTOR_DOWN(0.0f, -1.0f, 0.0f);
    static const vec3f VECTOR_LEFT(-1.0f, 0.0f, 0.0f);
    static const vec3f VECTOR_UP(0.0f, 1.0f, 0.0f);
    static const vec3f VECTOR_FORWARD(0.0f, 0.0f, 1.0f);
    static const vec3f VECTOR_BACKWARD(0.0f, 0.0f, -1.0f);


    template<typename _LeftType, typename _RightType>
    auto Dot(const Vector<_LeftType, 3>& a, const Vector<_RightType, 3>& b) noexcept -> decltype(a.x * b.x) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template<typename _LeftType, typename _RightType>
    auto Cross(const Vector<_LeftType, 3>& a, const Vector<_RightType, 3>& b) noexcept -> Vector<decltype(a.x * b.x), 3> {
        return Vector<decltype(a.x * b.x), 3>(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
}
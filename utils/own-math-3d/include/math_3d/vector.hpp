#pragma once
#include <type_traits>
#include <cassert>
#include <utility>

namespace math {
    template <typename Type, std::size_t Dimension>
    struct Vector {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Vector: Type must be an arithmetic");
        static_assert(Dimension >= 2 && Dimension <= 4, "template <typename Type, std::size_t Dimension> struct Vector: Dimension must be between 2 and 4");
    };

    template <typename Type>
    struct Vector<Type, 4> {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Vector: Type must be an arithmetic");
        
        using type = typename Type;
        static constexpr std::size_t dimension = 4;
    
    #pragma region interface
        Vector() 
            : Vector(0.0f) { }
        Vector(Type _x, Type _y, Type _z, Type _w = Type(0)) 
            : x(_x), y(_y), z(_z), w(_w) { }
        explicit Vector(Type value) 
            : x(value), y(value), z(value), w(value) { }
        Vector(const Vector<Type, 3>& vec, Type _w = Type(0)) 
            : x(vec.x), y(vec.y), z(vec.z), w(_w) { }
        Vector(const Vector<Type, 2>& vec, Type _z = Type(0), Type _w = Type(0)) 
            : x(vec.x), y(vec.y), z(_z), w(_w) { }

        float Length() const noexcept {
            return sqrtf(x * x + y * y + z * z + w * w);
        }

        bool operator==(const Vector& vec) const noexcept {
            return IsTendsTo(x, vec.x) 
                && IsTendsTo(y, vec.y) 
                && IsTendsTo(z, vec.z) 
                && IsTendsTo(w, vec.w);
        }

        bool operator!=(const Vector& vec) const noexcept {
            return !(*this == vec);
        }

        template <typename RightType>
        Vector& operator=(const Vector<RightType, 4> &vec) noexcept {
            x = static_cast<type>(vec.x);
            y = static_cast<type>(vec.y);
            z = static_cast<type>(vec.z);
            w = static_cast<type>(vec.w);
            return *this;
        }

        template <typename RightType>
        Vector& operator=(const Vector<RightType, 3> &vec) noexcept {
            x = static_cast<type>(vec.x);
            y = static_cast<type>(vec.y);
            z = static_cast<type>(vec.z);
            w = 0.0f;
            return *this;
        }

        template <typename RightType>
        Vector& operator=(const Vector<RightType, 2> &vec) noexcept {
            x = static_cast<type>(vec.x);
            y = static_cast<type>(vec.y);
            z = 0.0f;
            w = 0.0f;
            return *this;
        }

        Vector operator+(const Vector &vec) const noexcept {
            return Vector(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
        }

        Vector operator-(const Vector &vec) const noexcept {
            return Vector(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
        }

        Vector &operator+=(const Vector &vec) noexcept {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            w += vec.w;
            return *this;
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

        friend Vector operator*(float value, const Vector& vector) noexcept {
            return vector * value;   
        }

        Vector operator/(float value) const noexcept {
            return Vector(x / value, y / value, z / value, w / value);
        }

        Vector& operator*=(float value) noexcept {
            x *= value;
            y *= value;
            z *= value;
            w *= value;
            return *this;
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
            assert(idx < dimension && "Type& Vector<Type, 4>::operator[](std::size_t idx): invalid idx value");
            return *(&x + idx);
        }

        Type operator[](std::size_t idx) const {
            assert(idx < dimension && "Type Vector<Type, 4>::operator[](std::size_t idx) const: invalid idx value");
            return *(&x + idx);
        }

    private:
        static Vector<Type, 4> _Random(Type min, Type max) noexcept {
            return Vector<Type, 4>(math::Random(min, max), math::Random(min, max), math::Random(min, max), math::Random(min, max));
        }

        template <typename Vec>
        friend auto RandomVector(typename Vec::type min, typename Vec::type max) noexcept -> Vector<typename Vec::type, Vec::dimension>;
    #pragma endregion interface

    public:
        Type x, y, z, w;
    };

    template <typename Type>
    struct Vector<Type, 3> {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Vector: Type must be an arithmetic");

        using type = typename Type;
        static constexpr std::size_t dimension = 3;
    
    #pragma region interface
        Vector() 
            : Vector(0.0f) { }
        Vector(Type _x, Type _y, Type _z) 
            : x(_x), y(_y), z(_z) { }
        explicit Vector(Type value) 
            : x(value), y(value), z(value) { }
        Vector(const Vector<Type, 4>& vec) 
            : x(vec.x), y(vec.y), z(vec.z) { }
        Vector(const Vector<Type, 2>& vec, Type _z = Type(0)) 
            : x(vec.x), y(vec.y), z(_z) { }

        float Length() const noexcept {
            return sqrtf(x * x + y * y + z * z);
        }

        bool operator==(const Vector& vec) const noexcept {
            return IsTendsTo(x, vec.x) && IsTendsTo(y, vec.y) && IsTendsTo(z, vec.z);
        }

        bool operator!=(const Vector& vec) const noexcept {
            return !(*this == vec);
        }

        template <typename RightType, std::size_t Dimension>
        Vector& operator=(const Vector<RightType, Dimension> &vec) noexcept {
            x = static_cast<type>(vec.x);
            y = static_cast<type>(vec.y);
            z = static_cast<type>(vec.z);
            return *this;
        }

        template <typename RightType>
        Vector& operator=(const Vector<RightType, 2> &vec) noexcept {
            x = static_cast<type>(vec.x);
            y = static_cast<type>(vec.y);
            z = 0.0f;
            return *this;
        }

        Vector operator+(const Vector &vec) const noexcept {
            return Vector(x + vec.x, y + vec.y, z + vec.z);
        }

        Vector operator-(const Vector &vec) const noexcept {
            return Vector(x - vec.x, y - vec.y, z - vec.z);
        }

        Vector &operator+=(const Vector &vec) noexcept {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            return *this;
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

        friend Vector operator*(float value, const Vector& vector) noexcept {
            return vector * value;   
        }

        Vector operator/(float value) const noexcept {
            return Vector(x / value, y / value, z / value);
        }

        Vector& operator*=(float value) noexcept {
            x *= value;
            y *= value;
            z *= value;
            return *this;
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
            assert(idx < dimension && "Type& Vector<Type, 3>::operator[](std::size_t idx): invalid idx value");
            return *(&x + idx);
        }
        Type operator[](std::size_t idx) const {
            assert(idx < dimension && "Type Vector<Type, 3>::operator[](std::size_t idx) const: invalid idx value");
            return *(&x + idx);
        }
    #pragma endregion interface

    private:
        static Vector<Type, 3> _Random(Type min, Type max) noexcept {
            return Vector<Type, 3>(math::Random(min, max), math::Random(min, max), math::Random(min, max));
        }

        template <typename Vec>
        friend auto RandomVector(typename Vec::type min, typename Vec::type max) noexcept -> Vector<typename Vec::type, Vec::dimension>;

    public:
        Type x, y, z;
    };

    template <typename Type>
    struct Vector<Type, 2> {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Vector: Type must be an arithmetic");

        using type = typename Type;
        static constexpr std::size_t dimension = 2;
    
    #pragma region interface
        Vector() 
            : Vector(Type(0)) { }
        Vector(Type _x, Type _y) 
            : x(_x), y(_y) { }
        explicit Vector(Type value) 
            : x(value), y(value) { }
        Vector(const Vector<Type, 4>& vec) 
            : x(vec.x), y(vec.y) { }
        Vector(const Vector<Type, 3>& vec) 
            : x(vec.x), y(vec.y) { }

        float Length() const noexcept {
            return sqrtf(x * x + y * y);
        }

        bool operator==(const Vector& vec) const noexcept {
            return IsTendsTo(x, vec.x) && IsTendsTo(y, vec.y);
        }

        bool operator!=(const Vector& vec) const noexcept {
            return !(*this == vec);
        }

        template <typename RightType, std::size_t Dimension>
        Vector& operator=(const Vector<RightType, Dimension>& vec) noexcept {
            x = static_cast<type>(vec.x);
            y = static_cast<type>(vec.y);
            return *this;
        }

        Vector operator+(const Vector &vec) const noexcept {
            return Vector(x + vec.x, y + vec.y);
        }

        Vector operator-(const Vector &vec) const noexcept {
            return Vector(x - vec.x, y - vec.y);
        }

        Vector &operator+=(const Vector &vec) noexcept {
            x += vec.x;
            y += vec.y;
            return *this;
        }

        Vector &operator-=(const Vector &vec) noexcept {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }

        Vector operator*(float value) const noexcept {
            return Vector(x * value, y * value);
        }

        friend Vector operator*(float value, const Vector& vector) noexcept {
            return vector * value;   
        }

        Vector operator/(float value) const noexcept {
            return Vector(x / value, y / value);
        }

        Vector& operator*=(float value) noexcept {
            x *= value;
            y *= value;
            return *this;
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
            assert(idx < dimension && "Type& Vector<Type, 2>::operator[](std::size_t idx): invalid idx value");
            return *(&x + idx);
        }
        Type operator[](std::size_t idx) const {
            assert(idx < dimension && "Type Vector<Type, 2>::operator[](std::size_t idx) const: invalid idx value");
            return *(&x + idx);
        }
    #pragma endregion interface

    private:
        static Vector<Type, 2> _Random(Type min, Type max) noexcept {
            return Vector<Type, 2>(math::Random(min, max), math::Random(min, max));
        }

        template <typename Vec>
        friend auto RandomVector(typename Vec::type min, typename Vec::type max) noexcept -> Vector<typename Vec::type, Vec::dimension>;

    public:
        Type x, y;
    };

    template <typename Vec>
    auto RandomVector(typename Vec::type min, typename Vec::type max) noexcept -> Vector<typename Vec::type, Vec::dimension> {
        return Vector<typename Vec::type, Vec::dimension>::_Random(min, max);
    }

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

    const vec3f VECTOR_ZERO(0.0f);
    const vec3f VECTOR_RIGHT(1.0f, 0.0f, 0.0f);
    const vec3f VECTOR_DOWN(0.0f, -1.0f, 0.0f);
    const vec3f VECTOR_LEFT(-1.0f, 0.0f, 0.0f);
    const vec3f VECTOR_UP(0.0f, 1.0f, 0.0f);
    const vec3f VECTOR_FORWARD(0.0f, 0.0f, 1.0f);
    const vec3f VECTOR_BACKWARD(0.0f, 0.0f, -1.0f);
}
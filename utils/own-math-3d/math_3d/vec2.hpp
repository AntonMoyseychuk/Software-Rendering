#pragma once
#include <type_traits>
#include <cassert>
#include <cmath>

#include "util.hpp"

namespace math {
    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    union vec2 {
        using type = typename Type;

        vec2() noexcept
            : x(Type(0)), y(Type(0)) 
        {}
        vec2(Type x, Type y) noexcept
            : x(x), y(y)
        {}
        explicit vec2(Type value) noexcept
            : x(value), y(value)
        {}
        explicit vec2(const Type* arr) noexcept
            : x(arr[0]), y(arr[1])
        {}

        // explicit vec2(const vec3f& vec) noexcept
        //     : x(Type(vec.x)), y(Type(vec.y)) {}
        // explicit vec2(const vec4f& vec) noexcept
        //     : x(Type(vec.x)), y(Type(vec.y)) {}
        //
        // vec2& operator=(const vec3f& vec) noexcept {
        //     x = Type(vec.x);
        //     y = Type(vec.y);
        //     return *this;
        // }
        // vec2& operator=(const vec4f& vec) noexcept {
        //     x = Type(vec.x);
        //     y = Type(vec.y);
        //     return *this;
        // }

        vec2 operator-() const noexcept {
            return vec2(-x, -y);
        }

        template <typename Right>
        vec2 operator+(const vec2<Right>& vec) const noexcept {
            return vec2(x + Type(vec.x), y + Type(vec.y));
        }
        template <typename Right>
        vec2 operator-(const vec2<Right>& vec) const noexcept {
            return vec2(x - Type(vec.x), y - Type(vec.y));
        }
        template <typename Right>
        vec2& operator+=(const vec2<Right>& vec) noexcept {
            x += Type(vec.x);
            y += Type(vec.y);
            return *this;
        }
        template <typename Right>
        vec2& operator-=(const vec2<Right>& vec) noexcept {
            x += Type(vec.x);
            y += Type(vec.y);
            return *this;
        }

        template <typename Right>
        vec2 operator*(const vec2<Right>& vec) const noexcept {
            return vec2(Type(x * vec.x), Type(y * vec.y));
        }
        template <typename Right>
        vec2& operator*=(const vec2<Right>& vec) noexcept {
            x *= Type(vec.x);
            y *= Type(vec.y);
            return *this;
        }

        vec2 operator*(Type value) const noexcept {
            return vec2(Type(x * value), Type(y * value));
        }
        vec2& operator*=(Type value) noexcept {
            x *= value;
            y *= value;
            return *this;
        }
        vec2 operator/(Type value) const noexcept {
            if constexpr(std::is_floating_point_v<Type>) {
                assert(!is_tends_to(value, 0.0f));
            } else {
                assert(value != 0);
            }

            return vec2(x / value, y / value);
        }
        vec2& operator/=(Type value) noexcept {
            if constexpr(std::is_floating_point_v<Type>) {
                assert(!is_tends_to(value, 0.0f));
            } else {
                assert(value != 0);
            }

            x /= value;
            y /= value;
            return *this;
        }

        template <typename Right>
        bool operator==(const vec2<Right>& vec) const noexcept {
            if constexpr(std::is_floating_point_v<Type> && std::is_floating_point_v<Right>) {
                return is_tends_to(x, vec.x) && is_tends_to(y, vec.y);
            } else {
                return x == vec.x && y == vec.y;
            }
        }
        template <typename Right>
        bool operator!=(const vec2<Right>& vec) const noexcept {
            return !(*this == vec);
        }

        float length() const noexcept {
            std::sqrtf(x * x + y * y);
        }

        Type& operator[](size_t i) noexcept {
            assert(i < 2);
            return arr[i];
        }
        Type operator[](size_t i) const noexcept {
            assert(i < 2);
            return arr[i];
        }

        static vec2 get_random_vector(Type min, Type max) noexcept {
            return vec2f(random(min, max), random(min, max));
        }

        static const vec2& ZERO() noexcept {
            static vec2 zero(Type(0));
            return zero;
        } 
        static const vec2& LEFT() noexcept {
            assert(std::is_signed_v<Type>);
            static vec2 left(Type(-1), Type(0));
            return left;
        }
        static const vec2& RIGHT() noexcept {
            assert(std::is_signed_v<Type>);
            static vec2 right(Type(1), Type(0));
            return right;
        }
        static const vec2& UP() noexcept {
            assert(std::is_signed_v<Type>);
            static vec2 up(Type(0), Type(1));
            return up;
        }
        static const vec2& DOWN() noexcept {
            assert(std::is_signed_v<Type>);
            static vec2 down(Type(0), Type(-1));
            return down;
        }

        struct {
            Type x, y;
        };

        Type arr[2];
    };

    template <typename Type>
    vec2<Type> operator*(Type value, const vec2<Type>& vec) noexcept {
        return vec * value;
    }

    template <typename Type>
    vec2<float> normalize(const vec2<Type>& vec) noexcept {
        const float length = vec.length();
        return vec2<float>(vec.x / length, vec.y / length);
    }

    using vec2f = vec2<float>;
    using vec2i = vec2<int32_t>;
}
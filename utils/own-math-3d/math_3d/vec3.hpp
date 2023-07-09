#pragma once
#include "vec2.hpp"

namespace math {
    template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    struct vec3 {
        using type = typename Type;

        vec3() noexcept
            : x(0), y(0), z(0)
        {}
        vec3(Type x, Type y, Type z) noexcept
            : x(x), y(y), z(z)
        {}
        explicit vec3(Type value) noexcept
            : x(value), y(value), z(value)
        {}
        explicit vec3(const Type* arr) noexcept
            : x(arr[0]), y(arr[1]), z(arr[2])
        {}
        template<typename Right>
        explicit vec3(const vec3<Right>& vec) noexcept
            : x(vec.x), y(vec.y), z(vec.z)
        {}
        template<typename Right>
        vec3(const vec2<Right>& xy, Type z) noexcept
            : xy(xy), z(z)
        {}
        template<typename Right>
        vec3(Type x, const vec2<Right>& yz) noexcept
            : x(x), yz(yz)
        {}

        
        template<typename Right>
        vec3& operator=(const vec3<Right>& vec) noexcept {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            return *this;
        }

        template <typename = std::enable_if_t<std::is_signed_v<Type>>>
        vec3 operator-() const noexcept {
            return vec3(-x, -y, -z);
        }
        template <typename Right>
        vec3 operator+(const vec3<Right>& vec) const noexcept {
            return vec3(x + vec.x, y + vec.y, z + vec.z);
        }
        template <typename Right>
        vec3 operator-(const vec3<Right>& vec) const noexcept {
            return vec3(x - vec.x, y - vec.y, z - vec.z);
        }
        template <typename Right>
        vec3& operator+=(const vec3<Right>& vec) noexcept {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            return *this;
        }
        template <typename Right>
        vec3& operator-=(const vec3<Right>& vec) noexcept {
            x -= vec.x;
            y -= vec.y;
            y -= vec.y;
            return *this;
        }

        template <typename Right>
        vec3 operator*(const vec3<Right>& vec) const noexcept {
            return vec3(x * vec.x, y * vec.y, z * vec.z);
        }
        template <typename Right>
        vec3& operator*=(const vec3<Right>& vec) noexcept {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            return *this;
        }

        vec3 operator*(Type value) const noexcept {
            return vec3(x * value, y * value, z * value);
        }
        vec3& operator*=(Type value) noexcept {
            x *= value;
            y *= value;
            z *= value;
            return *this;
        }
        vec3 operator/(Type value) const noexcept {
            if constexpr(std::is_floating_point_v<Type>) {
                assert(!is_tends_to(value, 0.0f));
            } else {
                assert(value != 0);
            }

            return vec3(x / value, y / value, z / value);
        }
        vec3& operator/=(Type value) noexcept {
            if constexpr(std::is_floating_point_v<Type>) {
                assert(!is_tends_to(value, 0.0f));
            } else {
                assert(value != 0);
            }

            x /= value;
            y /= value;
            z /= value;
            return *this;
        }

        template <typename Right>
        bool operator==(const vec3<Right>& vec) const noexcept {
            if constexpr(std::is_floating_point_v<Type> && std::is_floating_point_v<Right>) {
                return is_tends_to(x, vec.x) && is_tends_to(y, vec.y) && is_tends_to(z, vec.z);
            } else {
                return x == vec.x && y == vec.y && z == vec.z;
            }
        }
        template <typename Right>
        bool operator!=(const vec3<Right>& vec) const noexcept {
            return !this->operator==(vec);
        }

        float length() const noexcept {
            return std::sqrtf(x * x + y * y + z * z);
        }

        Type& operator[](size_t i) noexcept {
            assert(i < 3);
            return arr[i];
        }
        Type operator[](size_t i) const noexcept {
            assert(i < 3);
            return arr[i];
        }

        static vec3 get_random_vector(Type min, Type max) noexcept {
            return vec3(random(min, max), random(min, max), random(min, max));
        }

        static const vec3& ZERO() noexcept {
            static const vec3 zero;
            return zero;
        }
        template <typename = std::enable_if_t<std::is_signed_v<Type>>>
        static const vec3& LEFT() noexcept {
            static const vec3 left(-1, 0, 0);
            return left;
        }
        static const vec3& RIGHT() noexcept {
            static const vec3 right(1, 0, 0);
            return right;
        }
        static const vec3& UP() noexcept {
            static const vec3 up(0, 1, 0);
            return up;
        }
        template <typename = std::enable_if_t<std::is_signed_v<Type>>>
        static const vec3& DOWN() noexcept {
            static const vec3 down(0, -1, 0);
            return down;
        }
        static const vec3& FORWARD() noexcept {
            static const vec3 forward(0, 0, 1);
            return forward;
        }
        template <typename = std::enable_if_t<std::is_signed_v<Type>>>
        static const vec3& BACKWARD() noexcept {
            static const vec3 backward(0, 0, -1);
            return backward;
        }


        union {
            struct {
                Type x, y, z;
            };

            struct {
                vec2<Type> xy;
                Type _ignore_0;
            };

            struct {
                Type _ignore_1;
                vec2<Type> yz;
            };

            Type arr[3];
        };
    };

    template <typename Type>
    vec3<Type> operator*(Type value, const vec3<Type>& vec) noexcept {
        return vec * value;
    }

    template <typename Type>
    vec3<float> normalize(const vec3<Type>& vec) noexcept {
        const float length = vec.length();
        return vec3<float>(vec.x / length, vec.y / length, vec.z / length);
    }
    
    using vec3f = vec3<float>;
    using vec3d = vec3<double>;
    using vec3i = vec3<int32_t>;
    using vec3ui = vec3<uint32_t>;
}
#pragma once
#include "math_3d/vector.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    struct Color {
        Color() = default;
        explicit Color(std::uint8_t value);
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);

        Color& operator*=(float value) noexcept;
        Color operator*(float value) const noexcept;

        Color& operator/=(float value) noexcept;
        Color operator/(float value) const noexcept;

        Color& operator+=(const Color& color) noexcept;
        Color operator+(const Color& color) const noexcept;

        Color& operator-=(const Color& color) noexcept;
        Color operator-(const Color& color) const noexcept;

        bool operator==(const Color& color) const noexcept;
        bool operator!=(const Color& color) const noexcept;

        template <typename Type>
        math::Vector<Type, 4> ToVector4() const noexcept {
            return math::Vector<Type, 4>(r, g, b, a);
        }

        template <>
        math::Vector<float, 4> ToVector4() const noexcept {
            return math::Vector<float, 4>(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }

        template <>
        math::Vector<double, 4> ToVector4() const noexcept {
            return math::Vector<double, 4>(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
        }

        template <typename Type>
        const Color& LoadColorFromVector(const math::Vector<Type, 4>& vec) noexcept {
            r = math::Clamp(vec.x, 0, 255);
            g = math::Clamp(vec.y, 0, 255);
            b = math::Clamp(vec.z, 0, 255);
            a = math::Clamp(vec.w, 0, 255);
            return *this;
        }

        template <>
        const Color& LoadColorFromVector(const math::Vector<float, 4>& vec) noexcept {
            r = static_cast<std::uint8_t>(math::Clamp(vec.x * 255.0f, 0.0f, 255.0f)); 
            g = static_cast<std::uint8_t>(math::Clamp(vec.y * 255.0f, 0.0f, 255.0f)); 
            b = static_cast<std::uint8_t>(math::Clamp(vec.z * 255.0f, 0.0f, 255.0f)); 
            a = static_cast<std::uint8_t>(math::Clamp(vec.w * 255.0f, 0.0f, 255.0f));
            return *this;
        }

        template <>
        const Color& LoadColorFromVector(const math::Vector<double, 4>& vec) noexcept {
            r = static_cast<std::uint8_t>(math::Clamp(vec.x * 255.0, 0.0, 255.0)); 
            g = static_cast<std::uint8_t>(math::Clamp(vec.y * 255.0, 0.0, 255.0)); 
            b = static_cast<std::uint8_t>(math::Clamp(vec.z * 255.0, 0.0, 255.0)); 
            a = static_cast<std::uint8_t>(math::Clamp(vec.w * 255.0, 0.0, 255.0));
            return *this;
        }

        const Color& LoadColorFromUInt32(std::uint32_t color) noexcept {
            this->rgba = color;
            return *this;
        }

        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color MAGENTA;
        static const Color CYAN;
        static const Color WHITE;
        static const Color BLACK;

        union {
            struct {
                std::uint8_t r, g, b, a;
            };

            std::uint32_t rgba; //endian dependent
        };
    };
}
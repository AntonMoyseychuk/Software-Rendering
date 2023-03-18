#pragma once
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

    template <typename Type>
    inline math::Vector<Type, 4> ToVector4(Color color) noexcept {
        return math::Vector<Type, 4>(color.r, color.g, color.b, color.a);
    }

    template <>
    inline math::Vector<float, 4> ToVector4(Color color) noexcept {
        return math::Vector<float, 4>(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    }

    template <>
    inline math::Vector<double, 4> ToVector4(Color color) noexcept {
        return math::Vector<double, 4>(color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0);
    }

    template <typename Type>
    inline Color LoadColorFromVector(const math::Vector<Type, 4>& vec) noexcept {
        auto r = math::Clamp(vec.x, static_cast<Type>(0), static_cast<Type>(255));
        auto g = math::Clamp(vec.y, static_cast<Type>(0), static_cast<Type>(255));
        auto b = math::Clamp(vec.z, static_cast<Type>(0), static_cast<Type>(255));
        auto a = math::Clamp(vec.w, static_cast<Type>(0), static_cast<Type>(255));
        return Color(r, g, b, a);
    }

    template <>
    inline Color LoadColorFromVector(const math::Vector<float, 4>& vec) noexcept {
        auto r = static_cast<std::uint8_t>(math::Clamp(vec.x * 255.0f, 0.0f, 255.0f)); 
        auto g = static_cast<std::uint8_t>(math::Clamp(vec.y * 255.0f, 0.0f, 255.0f)); 
        auto b = static_cast<std::uint8_t>(math::Clamp(vec.z * 255.0f, 0.0f, 255.0f)); 
        auto a = static_cast<std::uint8_t>(math::Clamp(vec.w * 255.0f, 0.0f, 255.0f));
        return Color(r, g, b, a);
    }

    template <>
    inline Color LoadColorFromVector(const math::Vector<double, 4>& vec) noexcept {
        auto r = static_cast<std::uint8_t>(math::Clamp(vec.x * 255.0, 0.0, 255.0)); 
        auto g = static_cast<std::uint8_t>(math::Clamp(vec.y * 255.0, 0.0, 255.0)); 
        auto b = static_cast<std::uint8_t>(math::Clamp(vec.z * 255.0, 0.0, 255.0)); 
        auto a = static_cast<std::uint8_t>(math::Clamp(vec.w * 255.0, 0.0, 255.0));
        return Color(r, g, b, a);
    }

    inline Color LoadColorFromUInt32(std::uint32_t color) noexcept {
        Color c;
        c.rgba = color;
        return c;
    }
}
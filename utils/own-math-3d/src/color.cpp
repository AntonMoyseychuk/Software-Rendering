#include "../include/math_3d/color.hpp"

#include <algorithm>

namespace math {
    const Color Color::RED(255u, 0u, 0u);
    const Color Color::GREEN(0u, 128u, 0u);
    const Color Color::BLUE(0u, 0u, 255u);
    const Color Color::YELLOW(255u, 255u, 0u);
    const Color Color::MAGENTA(255u, 0u, 255u);
    const Color Color::CYAN(0u, 255u, 255u);
    const Color Color::ORANGE(255u, 102u, 0u);
    const Color Color::LIME(0u, 255u, 0u);
    const Color Color::PURPLE(128u, 0u, 128u);
    const Color Color::INDIGO(75u, 0u, 130u);
    const Color Color::GOLDEN(218u, 165u, 32u);
    const Color Color::SKY_BLUE(135u, 206u, 235u);
    const Color Color::BLACK(0u, 0u, 0u);
    const Color Color::WHITE(255u, 255u, 255u);
    const Color Color::GREY(128u, 128u, 128u);

    Color::Color(std::uint8_t value, std::uint8_t a)
        : Color(value, value, value, a)
    {
    }

    Color::Color(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a)
        : r(_r), g(_g), b(_b), a(_a)
    {         
    }

    Color& Color::operator*=(float value) noexcept {
        r = static_cast<uint8_t>(math::Clamp(r * value, 0.0f, 255.0f));
        g = static_cast<uint8_t>(math::Clamp(g * value, 0.0f, 255.0f));
        b = static_cast<uint8_t>(math::Clamp(b * value, 0.0f, 255.0f));
        a = static_cast<uint8_t>(math::Clamp(a * value, 0.0f, 255.0f));
        return *this;
    }

    Color Color::operator*(float value) const noexcept {
        return Color(
            static_cast<uint8_t>(math::Clamp(r * value, 0.0f, 255.0f)),
            static_cast<uint8_t>(math::Clamp(g * value, 0.0f, 255.0f)),
            static_cast<uint8_t>(math::Clamp(b * value, 0.0f, 255.0f)),
            static_cast<uint8_t>(math::Clamp(a * value, 0.0f, 255.0f))
        );
    }

    Color& Color::operator*=(Color color) noexcept {
        r = static_cast<uint8_t>(this->R() * color.R() * 255.0f);
        g = static_cast<uint8_t>(this->G() * color.G() * 255.0f);
        b = static_cast<uint8_t>(this->B() * color.B() * 255.0f);
        a = static_cast<uint8_t>(this->A() * color.A() * 255.0f);
        return *this;
    }

    Color Color::operator*(Color color) const noexcept {
        return Color(
            static_cast<uint8_t>(this->R() * color.R() * 255.0f),
            static_cast<uint8_t>(this->G() * color.G() * 255.0f),
            static_cast<uint8_t>(this->B() * color.B() * 255.0f),
            static_cast<uint8_t>(this->A() * color.A() * 255.0f)
        );
    }

    Color &Color::operator/=(float value) noexcept {
        r = static_cast<uint8_t>(math::Clamp(r / value, 0.0f, 255.0f));
        g = static_cast<uint8_t>(math::Clamp(g / value, 0.0f, 255.0f));
        b = static_cast<uint8_t>(math::Clamp(b / value, 0.0f, 255.0f));
        a = static_cast<uint8_t>(math::Clamp(a / value, 0.0f, 255.0f));

        return *this;
    }

    Color Color::operator/(float value) const noexcept {
        return Color(
            static_cast<uint8_t>(math::Clamp(r / value, 0.0f, 255.0f)),
            static_cast<uint8_t>(math::Clamp(g / value, 0.0f, 255.0f)),
            static_cast<uint8_t>(math::Clamp(b / value, 0.0f, 255.0f)),
            static_cast<uint8_t>(math::Clamp(a / value, 0.0f, 255.0f))
        );
    }

    Color &Color::operator+=(Color c) noexcept {
        r = math::Clamp(r + c.r, 0, 255);
        g = math::Clamp(g + c.g, 0, 255);
        b = math::Clamp(b + c.b, 0, 255);
        a = math::Clamp(a + c.a, 0, 255);

        return *this;
    }
    
    Color Color::operator+(Color c) const noexcept {
        return Color(
            math::Clamp(r + c.r, 0, 255),
            math::Clamp(g + c.g, 0, 255),
            math::Clamp(b + c.b, 0, 255),
            math::Clamp(a + c.a, 0, 255)
        );
    }

    Color &Color::operator-=(Color c) noexcept {
        r = math::Clamp(r - c.r, 0, 255);
        g = math::Clamp(g - c.g, 0, 255);
        b = math::Clamp(b - c.b, 0, 255);
        a = math::Clamp(a - c.a, 0, 255);

        return *this;
    }

    Color Color::operator-(Color c) const noexcept {
        return Color(
            math::Clamp(r - c.r, 0, 255),
            math::Clamp(g - c.g, 0, 255),
            math::Clamp(b - c.b, 0, 255),
            math::Clamp(a - c.a, 0, 255)
        );
    }

    bool Color::operator==(Color color) const noexcept {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }
    
    bool Color::operator!=(Color color) const noexcept {
        return !(*this == color);
    }

    Color operator*(float value, Color color) noexcept  {
        return color * value;
    }

    Color UInt32ToColor(std::uint32_t color) noexcept
    {
        Color c;
        c.rgba = color;
        return c;
    }
}
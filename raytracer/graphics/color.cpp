#include "color.hpp"

#include <algorithm>

namespace gfx {
    const Color Color::BLACK(0u, 0u, 0u);
    const Color Color::WHITE(255u, 255u, 255u);
    const Color Color::RED(255u, 0u, 0u);
    const Color Color::GREEN(0u, 255u, 0u);
    const Color Color::BLUE(0u, 0u, 255u);
    const Color Color::YELLOW(255u, 255u, 0u);
    const Color Color::MAGENTA(255u, 0u, 255u);
    const Color Color::CYAN(0u, 255u, 255u);

    Color::Color(std::uint8_t value)
        : Color(value, value, value, value)
    {
    }

    Color::Color(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a)
        : r(_r), g(_g), b(_b), a(_a)
    {         
    }

    Color& Color::operator*=(float value) noexcept {
        r = std::clamp(static_cast<int>(r * value), 0, 255);
        g = std::clamp(static_cast<int>(g * value), 0, 255);
        b = std::clamp(static_cast<int>(b * value), 0, 255);
        a = std::clamp(static_cast<int>(a * value), 0, 255);
        return *this;
    }

    Color Color::operator*(float value) const noexcept {
        return Color(
            std::clamp(static_cast<int>(r * value), 0, 255),
            std::clamp(static_cast<int>(g * value), 0, 255),
            std::clamp(static_cast<int>(b * value), 0, 255),
            std::clamp(static_cast<int>(a * value), 0, 255)
        );
    }

    Color &Color::operator/=(float value) noexcept {
        r = std::clamp(static_cast<int>(r / value), 0, 255);
        g = std::clamp(static_cast<int>(g / value), 0, 255);
        b = std::clamp(static_cast<int>(b / value), 0, 255);
        a = std::clamp(static_cast<int>(a / value), 0, 255);

        return *this;
    }

    Color Color::operator/(float value) const noexcept {
        return Color(
            std::clamp(static_cast<int>(r / value), 0, 255),
            std::clamp(static_cast<int>(g / value), 0, 255),
            std::clamp(static_cast<int>(b / value), 0, 255),
            std::clamp(static_cast<int>(a / value), 0, 255)
        );
    }

    Color &Color::operator+=(const Color &c) noexcept {
        r = std::clamp(r + c.r, 0, 255);
        g = std::clamp(g + c.g, 0, 255);
        b = std::clamp(b + c.b, 0, 255);
        a = std::clamp(a + c.a, 0, 255);

        return *this;
    }
    
    Color Color::operator+(const Color &c) const noexcept {
        return Color(
            std::clamp(r + c.r, 0, 255),
            std::clamp(g + c.g, 0, 255),
            std::clamp(b + c.b, 0, 255),
            std::clamp(a + c.a, 0, 255)
        );
    }

    Color &Color::operator-=(const Color &c) noexcept {
        r = std::clamp(r - c.r, 0, 255);
        g = std::clamp(g - c.g, 0, 255);
        b = std::clamp(b - c.b, 0, 255);
        a = std::clamp(a - c.a, 0, 255);

        return *this;
    }

    Color Color::operator-(const Color &c) const noexcept {
        return Color(
            std::clamp(r - c.r, 0, 255),
            std::clamp(g - c.g, 0, 255),
            std::clamp(b - c.b, 0, 255),
            std::clamp(a - c.a, 0, 255)
        );
    }

    bool Color::operator==(const Color &color) const noexcept {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }
    
    bool Color::operator!=(const Color &color) const noexcept {
        return r != color.r && g != color.g && b != color.b && a != color.a;
    }
}
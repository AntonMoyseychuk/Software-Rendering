#pragma once
#include "declarations.hpp"

namespace math {
    struct Color {
        Color() = default;
        Color(std::uint8_t value);
        Color(const Vec3f& vec);
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);

        Color& operator*=(float value) noexcept;
        Color operator*(float value) const noexcept;

        Color& operator/=(float value) noexcept;
        Color operator/(float value) const noexcept;

        Color& operator+=(const Color& color) noexcept;
        Color operator+(const Color& color) const noexcept;

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

            uint32_t rgba;
        };
    };
}
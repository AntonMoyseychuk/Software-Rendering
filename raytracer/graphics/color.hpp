#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    struct Color {
        Color() = default;
        explicit Color(std::uint32_t value);
        Color(const math::vec4f& vec);
        Color(const math::vec4ub& vec);
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

        operator math::vec4ub() const noexcept;

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
#include "declarations.hpp"

namespace math {
    struct Color {
        Color() = default;
        Color(std::uint8_t value);
        Color(const Vector3& vec);
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);

        Color& operator*=(float value) noexcept;
        Color operator*(float value) const noexcept;

        Color& operator+=(const Color& c) noexcept;
        Color operator+(const Color& c) const noexcept;

        std::uint8_t r, g, b, a;
    };
}
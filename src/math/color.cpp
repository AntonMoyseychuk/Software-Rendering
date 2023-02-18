#include "color.hpp"
#include "vector.hpp"

#include <algorithm>

namespace math {
    Color::Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) 
        : r(_r), g(_g), b(_b), a(_a) 
    {         
    }

    Color::Color(uint8_t value)
        : Color(value, value, value, value) 
    {
    }
    
    Color::Color(const Vector3 &vec)
        : Color(std::clamp(vec.X() * 255, 0.f, 255.f), std::clamp(vec.Y() * 255, 0.f, 255.f), std::clamp(vec.Z() * 255, 0.f, 255.f))
    {
    }

    Color &Color::operator*=(float value) noexcept {
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
}
#pragma once
#include "math_3d/math.hpp"

namespace gfx {
    struct Color {
        Color() = default;
        explicit Color(std::uint8_t value, std::uint8_t a = 255u);
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255u);

        Color& operator*=(float value) noexcept;
        Color operator*(float value) const noexcept;

        Color& operator*=(Color color) noexcept;
        Color operator*(Color color) const noexcept;

        Color& operator/=(float value) noexcept;
        Color operator/(float value) const noexcept;

        Color& operator+=(Color color) noexcept;
        Color operator+(Color color) const noexcept;

        Color& operator-=(Color color) noexcept;
        Color operator-(Color color) const noexcept;

        bool operator==(Color color) const noexcept;
        bool operator!=(Color color) const noexcept;

        float R() const noexcept { return r / 255.0f; }
        void R(float r) noexcept { this->r = static_cast<std::uint8_t>(math::Clamp(r * 255.0f, 0.0f, 255.0f)); }

        float G() const noexcept { return g / 255.0f; }
        void G(float g) noexcept { this->g = static_cast<std::uint8_t>(math::Clamp(g * 255.0f, 0.0f, 255.0f)); }

        float B() const noexcept { return b / 255.0f; }
        void B(float b) noexcept { this->b = static_cast<std::uint8_t>(math::Clamp(b * 255.0f, 0.0f, 255.0f)); }

        float A() const noexcept { return a / 255.0f; }
        void A(float a) noexcept { this->a = static_cast<std::uint8_t>(math::Clamp(a * 255.0f, 0.0f, 255.0f)); }

        union {
            struct {
                std::uint8_t r, g, b, a;
            };

            std::uint32_t rgba; //endian dependent
        };

        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color MAGENTA;
        static const Color CYAN;
        static const Color ORANGE;
        static const Color LIME;
        static const Color PURPLE;
        static const Color INDIGO;
        static const Color GOLDEN;
        static const Color SKY_BLUE;
        static const Color WHITE;
        static const Color BLACK;
        static const Color GREY;
    };

    Color UInt32ToColor(std::uint32_t color) noexcept;
    
    template <typename Type>
    inline Color VectorToColor(const math::Vector<Type, 4>& vec) noexcept {
        auto r = static_cast<uint8_t>(math::Clamp(vec.x, Type(0), Type(255)));
        auto g = static_cast<uint8_t>(math::Clamp(vec.y, Type(0), Type(255)));
        auto b = static_cast<uint8_t>(math::Clamp(vec.z, Type(0), Type(255)));
        auto a = static_cast<uint8_t>(math::Clamp(vec.w, Type(0), Type(255)));
        return Color(r, g, b, a);
    }

    template <>
    inline Color VectorToColor(const math::Vector<float, 4>& vec) noexcept {
        auto r = static_cast<uint8_t>(math::Clamp(vec.x * 255.0f, 0.0f, 255.0f)); 
        auto g = static_cast<uint8_t>(math::Clamp(vec.y * 255.0f, 0.0f, 255.0f)); 
        auto b = static_cast<uint8_t>(math::Clamp(vec.z * 255.0f, 0.0f, 255.0f)); 
        auto a = static_cast<uint8_t>(math::Clamp(vec.w * 255.0f, 0.0f, 255.0f));
        return Color(r, g, b, a);
    }

    template <>
    inline Color VectorToColor(const math::Vector<double, 4>& vec) noexcept {
        auto r = static_cast<uint8_t>(math::Clamp(vec.x * 255.0, 0.0, 255.0)); 
        auto g = static_cast<uint8_t>(math::Clamp(vec.y * 255.0, 0.0, 255.0)); 
        auto b = static_cast<uint8_t>(math::Clamp(vec.z * 255.0, 0.0, 255.0)); 
        auto a = static_cast<uint8_t>(math::Clamp(vec.w * 255.0, 0.0, 255.0));
        return Color(r, g, b, a);
    }


    template <typename Type>
    inline math::Vector<Type, 4> ColorToVector(Color color) noexcept {
        return math::Vector<Type, 4>(color.r, color.g, color.b, color.a);
    }

    template <>
    inline math::Vector<float, 4> ColorToVector(Color color) noexcept {
        return math::Vector<float, 4>(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    }

    template <>
    inline math::Vector<double, 4> ColorToVector(Color color) noexcept {
        return math::Vector<double, 4>(color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0);
    }
}
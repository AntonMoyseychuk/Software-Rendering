#pragma once
#include "material.hpp"

namespace gfx {
    class IColoredMaterial : public IMaterial {
    public:
        IColoredMaterial() = default;
        IColoredMaterial(Color color) : m_color(color) {}

        void SetColor(Color color) noexcept { m_color = color; }
        Color GetColor() const noexcept { return m_color; }
    protected:
        Color m_color;
    };
}
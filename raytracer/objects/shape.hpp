#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    class IShape {
    public:
        IShape() = default;
        IShape(const math::vec3f& position) : m_position(position) {}
        virtual ~IShape() = 0 {}

        void MoveFor(const math::vec3f& dist) noexcept { m_position += dist; }
        
    #pragma region getters-setters
        void SetPositon(const math::vec3f& position) noexcept { m_position = position; }
        const math::vec3f& GetPositon() const noexcept { return m_position; }
        math::vec3f& GetPositon() noexcept { return m_position; }
    #pragma endregion getters-setters

    protected:
        math::vec3f m_position;
    };
}
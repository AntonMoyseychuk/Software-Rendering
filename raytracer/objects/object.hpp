#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    class IObject {
    public:
        IObject() = default;
        IObject(const math::vec4f& position) : m_position(position) {}
        virtual ~IObject() = 0 {}

        void SetPositon(const math::vec4f& position) noexcept { m_position = position; }
        void MoveFor(const math::vec4f& dist) noexcept { m_position += dist; }
        const math::vec4f& GetPositon() const noexcept { return m_position; }
        math::vec4f& GetPositon() noexcept { return m_position; }

    protected:
        math::vec4f m_position;
    };
}
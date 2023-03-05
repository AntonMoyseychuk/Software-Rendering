#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    class Object {
    public:
        Object() = default;
        Object(const math::vec4f& position) : m_position(position) {}
        virtual ~Object() = 0 {}

        void SetPositon(const math::vec4f& position) noexcept { m_position = position; }
        const math::vec4f& GetPositon() const noexcept { return m_position; }
        math::vec4f& GetPositon() noexcept { return m_position; }

    protected:
        math::vec4f m_position;
    };
}
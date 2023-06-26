#pragma once
#include "uniform_buffer.hpp"

#include <any>

namespace gl {
    struct _shader : public _uniform_buffer {
        _shader() = default;
        virtual ~_shader() = default;

        virtual std::any vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel(const std::any& vs_out) const noexcept = 0;

        virtual void geometry() const noexcept;

        friend struct _render_engine;

    protected:
        mutable math::vec4f gl_Position;
    };
}
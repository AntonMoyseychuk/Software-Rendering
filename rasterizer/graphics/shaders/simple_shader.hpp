#pragma once
#include "core/shader.hpp"

namespace rasterization::gfx {
    struct SimpleShader : public _shader {
        math::vec4f vertex(const void* vertex) const noexcept;
        math::color pixel(const void* vertex) const noexcept;
    };
}
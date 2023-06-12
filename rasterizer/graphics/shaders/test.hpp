#pragma once
#include "core/abstract_shader.hpp"

namespace rasterization::gfx {
    struct SimpleAbstrShader : public _abstract_shader {
        math::vec4f vertex(const void* vertex) const noexcept;
        math::color pixel(const void* vertex) const noexcept;
    };
}
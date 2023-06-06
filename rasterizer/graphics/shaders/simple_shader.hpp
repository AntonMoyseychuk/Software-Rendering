#pragma once
#include "core/shader.hpp"

namespace rasterization::gfx {
    class SimpleShader : public shader {
    public:
        SimpleShader();

        math::vec4f vertex(const void* vertex) const override;
        math::vec4f pixel(const void* vertex) const override;
    };
}
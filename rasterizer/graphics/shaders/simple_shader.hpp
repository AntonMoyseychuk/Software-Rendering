#pragma once
#include "core/shader.hpp"

namespace rasterization::gfx {
    struct SimpleShader : public _shader {
        void vertex(const void* vertex) const noexcept override;
        math::color pixel(const void* vertex) const noexcept override;
    };
}
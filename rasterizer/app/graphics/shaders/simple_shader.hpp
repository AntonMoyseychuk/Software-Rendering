#pragma once
#include "core/shader.hpp"

namespace rasterization::gfx {
    struct SimpleShader : public _shader {
        std::any vertex(const void* vertex) const noexcept override;
        math::color pixel(const std::any& vs_out) const noexcept override;
    };
}
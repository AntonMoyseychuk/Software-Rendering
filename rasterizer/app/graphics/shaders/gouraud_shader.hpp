#pragma once
#include "core/shader.hpp"

namespace rasterization {
    struct GouraudShader : public gl::_shader {
        std::any vertex(const void* vertex) const noexcept override;
        math::color pixel(const std::any& vs_out) const noexcept override;
    };
}
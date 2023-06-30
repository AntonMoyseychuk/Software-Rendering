#pragma once
#include "core/shader.hpp"

namespace rasterization {
    struct GouraudShader : public gl::_shader {
        void vertex(const void* vertex) const noexcept override;
        math::color pixel() const noexcept override;
    };
}
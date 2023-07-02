#pragma once
#include "core/shader-engine-api/shader.hpp"

namespace rasterization {
    struct SimpleShader : public gl::_shader {
        math::vec4f vertex(const void* vertex, pd& _pd) const noexcept override;
        math::color pixel(const pd& _pd) const noexcept override;
    };
}
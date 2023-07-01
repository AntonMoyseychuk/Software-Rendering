#pragma once
#include "core/shader.hpp"

namespace rasterization {
    struct SimpleShader : public gl::_shader {
        math::vec4f vertex(const void* vertex, pipeline_data& pack) const noexcept override;
        math::color pixel(const pipeline_data& pack) const noexcept override;
    };
}
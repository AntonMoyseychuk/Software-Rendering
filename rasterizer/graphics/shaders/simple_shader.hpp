#pragma once
#include "core/shader.hpp"

namespace rasterization::gfx {
    struct SimpleShader final {
        static math::vec4f vertex(const uniform_buffer& uniform_buffer, const void* vertex);
        static math::vec4f pixel(const uniform_buffer& uniform_buffer, const void* vertex);
    };
}
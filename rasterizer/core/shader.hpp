#pragma once
#include <unordered_map>

#include "math_3d/math.hpp"

namespace rasterization::gfx {
    struct uniform_buffer final {
        std::unordered_map<std::string, math::mat4f> mat4f_uniforms;
        std::unordered_map<std::string, math::vec4f> vec4f_uniforms;
        std::unordered_map<std::string, math::vec3f> vec3f_uniforms;
        std::unordered_map<std::string, math::vec2f> vec2f_uniforms;

        std::unordered_map<std::string, float> float_uniforms;
    };

    class shader final {
        friend struct _shader_engine;

        shader() = default;
        shader(math::vec4f (*vertex)(const uniform_buffer&, const void*), math::color (*pixel)(const uniform_buffer&, const void*))
            : vertex(vertex), pixel(pixel) {}

    public:
        math::vec4f (*vertex)(const uniform_buffer& uniform_buffer, const void* vertex) = nullptr;
        math::color (*pixel)(const uniform_buffer& uniform_buffer, const void* vertex) = nullptr;
    };
}
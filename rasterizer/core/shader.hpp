#pragma once
#include <unordered_map>
#include <functional>

#include "math_3d/math.hpp"

namespace rasterization::gfx {
    struct uniform_buffer final {
        std::unordered_map<std::string, math::mat4f> mat4f_uniforms;
        std::unordered_map<std::string, math::vec4f> vec4f_uniforms;
        std::unordered_map<std::string, math::vec3f> vec3f_uniforms;
        std::unordered_map<std::string, math::vec2f> vec2f_uniforms;

        std::unordered_map<std::string, float> float_uniforms;
    };

    struct _shader final {
        _shader() = default;
        _shader(
            const std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)>& vertex,
            const std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)>& pixel
        ) : vertex(vertex), pixel(pixel) {}

    
        std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)> vertex;
        std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)> pixel;
    };
}
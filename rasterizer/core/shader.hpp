#pragma once
#include <unordered_map>

#include "math_3d/math.hpp"

namespace rasterization::gfx {
    struct _uniform_buffer final {
        std::unordered_map<std::string, math::mat4f> mat4f_uniforms;
        std::unordered_map<std::string, math::vec4f> vec4f_uniforms;
        std::unordered_map<std::string, math::vec3f> vec3f_uniforms;
        std::unordered_map<std::string, math::vec2f> vec2f_uniforms;

        std::unordered_map<std::string, float> float_uniforms;
    };

    class shader {
        friend struct _shader_engine;

    public:
        virtual math::vec4f vertex(const void* vertex) const = 0;
        virtual math::vec4f pixel(const void* vertex) const = 0;

    protected:
        _uniform_buffer* m_uniform_buffer = nullptr;
    };
}
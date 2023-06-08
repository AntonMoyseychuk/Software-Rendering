#include "simple_shader.hpp"
#include "core/shader_engine.hpp"

#include <cassert>

namespace rasterization::gfx {
    struct Vertex {
        math::vec3f position;
    };
    
    math::vec4f SimpleShader::vertex(const uniform_buffer &uniform_buffer, const void *vertex) {
        using namespace math;
        
        ASSERT_UNIFORM_VALIDITY(uniform_buffer.mat4f_uniforms, "model");
        ASSERT_UNIFORM_VALIDITY(uniform_buffer.mat4f_uniforms, "view");
        ASSERT_UNIFORM_VALIDITY(uniform_buffer.mat4f_uniforms, "projection");
        
        const Vertex* v = (const Vertex*)vertex;
        return v->position * 
            uniform_buffer.mat4f_uniforms.at("model") * 
            uniform_buffer.mat4f_uniforms.at("view") * 
            uniform_buffer.mat4f_uniforms.at("projection");
    }
    
    math::vec4f SimpleShader::pixel(const uniform_buffer &uniform_buffer, const void *vertex) {
        return math::vec4f();
    }
}
#include "simple_shader.hpp"
#include "core/shader_engine.hpp"

#include <cassert>

namespace rasterization::gfx {
    SimpleShader::SimpleShader()
        : shader()
    {
    }
    
    math::vec4f SimpleShader::vertex(const void *vertex) const {
        using namespace math;
        
        ASSERT_UNIFORM_VALIDITY(m_uniform_buffer->mat4f_uniforms, "model");
        ASSERT_UNIFORM_VALIDITY(m_uniform_buffer->mat4f_uniforms, "view");
        ASSERT_UNIFORM_VALIDITY(m_uniform_buffer->mat4f_uniforms, "projection");
        
        return *(const vec3f*)vertex * 
            m_uniform_buffer->mat4f_uniforms["model"] * 
            m_uniform_buffer->mat4f_uniforms["view"] * 
            m_uniform_buffer->mat4f_uniforms["projection"];
    }
    
    math::vec4f SimpleShader::pixel(const void *vertex) const {
        return math::vec4f();
    }
}
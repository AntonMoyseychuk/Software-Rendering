#include "shader.hpp"
#include "shader_engine_macros.hpp"

namespace rasterization::gfx {
    void _shader::geometry() const noexcept
    {
    }

    const math::mat4f &_shader::get_mat4_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.mat4f_uniforms, name);
        return m_uniforms.mat4f_uniforms.at(name);
    }
    
    const math::vec4f &_shader::get_vec4_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.vec4f_uniforms, name);
        return m_uniforms.vec4f_uniforms.at(name);
    }
    
    const math::vec3f &_shader::get_vec3_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.vec3f_uniforms, name);
        return m_uniforms.vec3f_uniforms.at(name);
    }

    const math::vec2f &_shader::get_vec2_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.vec2f_uniforms, name);
        return m_uniforms.vec2f_uniforms.at(name);
    }

    float _shader::get_float_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.float_uniforms, name);
        return m_uniforms.float_uniforms.at(name);
    }
}
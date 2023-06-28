#include "uniform_buffer_api.hpp"

namespace gl {
    const math::quaternion &_uniform_buffer_api::get_quaternion_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_quaternion, name);
        return m_uniforms.u_quaternion.at(name);
    }

    const math::mat4f &_uniform_buffer_api::get_mat4_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_mat4, name);
        return m_uniforms.u_mat4.at(name);
    }
    
    const math::vec4f &_uniform_buffer_api::get_vec4_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_vec4, name);
        return m_uniforms.u_vec4.at(name);
    }
    
    const math::vec3f &_uniform_buffer_api::get_vec3_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_vec3, name);
        return m_uniforms.u_vec3.at(name);
    }

    const math::vec2f &_uniform_buffer_api::get_vec2_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_vec2, name);
        return m_uniforms.u_vec2.at(name);
    }

    float _uniform_buffer_api::get_f32_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_f32, name);
        return m_uniforms.u_f32.at(name);
    }

    double _uniform_buffer_api::get_f64_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_f64, name);
        return m_uniforms.u_f64.at(name);
    }

    int8_t _uniform_buffer_api::get_i8_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_i8, name);
        return m_uniforms.u_i8.at(name);
    }

    int16_t _uniform_buffer_api::get_i16_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_i16, name);
        return m_uniforms.u_i16.at(name);
    }

    int32_t _uniform_buffer_api::get_i32_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_i32, name);
        return m_uniforms.u_i32.at(name);
    }

    int64_t _uniform_buffer_api::get_i64_uniform(const std::string &name) const noexcept {
        ASSERT_UNIFORM_VALIDITY(m_uniforms.u_i64, name);
        return m_uniforms.u_i64.at(name);
    }
}
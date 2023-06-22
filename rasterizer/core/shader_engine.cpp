#include "shader_engine.hpp"

namespace gl {
    _shader_engine &_shader_engine::get() noexcept {
        static _shader_engine engine;
        return engine;
    }

    size_t _shader_engine::create_shader(const std::shared_ptr<_shader> &shader) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (shader_programs.count(id) != 0);

        shader_programs[id] = shader_program { shader };

        return id;
    }

    void _shader_engine::bind_shader(size_t shader_id) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, shader_id);
        binded_shader = shader_id;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::quaternion &q) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_quaternion[uniform_name] = q;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::mat4f &mat) noexcept  {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_mat4[uniform_name] = mat;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::vec4f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_vec4[uniform_name] = vec;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::vec3f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_vec3[uniform_name] = vec;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::vec2f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_vec2[uniform_name] = vec;
    }

    void _shader_engine::uniform(const std::string &uniform_name, float value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_f32[uniform_name] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_name, double value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_f64[uniform_name] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_name, int8_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_i8[uniform_name] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_name, int16_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_i16[uniform_name] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_name, int32_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_i32[uniform_name] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_name, int64_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].shader->m_uniforms.u_i64[uniform_name] = value;
    }

    const _shader_engine::shader_program &_shader_engine::_get_binded_shader_program() const noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        return shader_programs.at(binded_shader);
    }
}
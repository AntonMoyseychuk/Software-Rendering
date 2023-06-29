#include "shader_engine.hpp"
#include "assert_macro.hpp"

#define ASSERT_SHADER_PROGRAM_ID_VALIDITY(container, program_id) ASSERT(container.find((program_id)) != container.cend(), "shader engine error", "invalid shader program ID")

namespace gl {
    _shader_engine &_shader_engine::get() noexcept {
        static _shader_engine engine;
        return engine;
    }

    size_t _shader_engine::create_shader(const std::shared_ptr<_shader> &shader) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_shader_programs.find(id) != m_shader_programs.cend());

        m_shader_programs[id] = shader_program { shader };

        return id;
    }

    void _shader_engine::bind_shader(size_t id) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, id);
        m_binded_shader = id;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, const math::quaternion &q) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = q;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, const math::mat4f &mat) noexcept  {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = mat;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, const math::vec4f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = vec;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, const math::vec3f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = vec;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, const math::vec2f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = vec;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, float value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, double value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, int8_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, int16_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, int32_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = value;
    }

    void _shader_engine::uniform(const std::string &uniform_tag, int64_t value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        m_shader_programs[m_binded_shader].shader->m_uniforms.u[uniform_tag] = value;
    }

    const _shader_engine::shader_program &_shader_engine::_get_binded_shader_program() const noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        return m_shader_programs.at(m_binded_shader);
    }
}
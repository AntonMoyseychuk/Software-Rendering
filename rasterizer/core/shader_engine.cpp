#include "shader_engine.hpp"
#include "shader.hpp"

#define _ASSERT_SHADER_PROGRAM_ID_VALIDITY(container, id) ASSERT(container.find((id)) != container.cend(), "shader engine error", "invalid shader program ID")

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

        m_shader_programs[id] = { uniforms_pack_type(), shader };

        return id;
    }

    void _shader_engine::bind_shader(size_t id) noexcept {
        _ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, id);
        m_binded_shader = id;
    }

    const _shader_engine::shader_program &_shader_engine::_get_binded_shader_program() const noexcept {
        _ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
        return m_shader_programs.at(m_binded_shader);
    }
}
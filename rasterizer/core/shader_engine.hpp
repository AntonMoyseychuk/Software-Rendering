#pragma once
#include "shader.hpp"
#include "assert_macro.hpp"

#define ASSERT_SHADER_PROGRAM_ID_VALIDITY(container, id) ASSERT(container.find((id)) != container.cend(), "shader engine error", "invalid shader program ID")

namespace gl {
    struct _shader_engine final {
        _shader_engine(const _shader_engine& engine) = delete;
        _shader_engine& operator=(const _shader_engine& engine) = delete;

        static _shader_engine& get() noexcept;

        size_t create_shader(const std::shared_ptr<_shader>& shader) noexcept;
        void bind_shader(size_t id) noexcept;

        template<typename Uniform>
        void uniform(const Uniform& uniform, const std::string& uniform_tag) noexcept {
            ASSERT_SHADER_PROGRAM_ID_VALIDITY(m_shader_programs, m_binded_shader);
            m_shader_programs[m_binded_shader].shader->_set_uniform(uniform, uniform_tag);
        }

    private:
        _shader_engine() = default;

    public:
        struct shader_program final {
            std::shared_ptr<_shader> shader;
        };
        const shader_program& _get_binded_shader_program() const noexcept;

        using shader_id = size_t;

    private:
        std::unordered_map<shader_id, shader_program> m_shader_programs;
        shader_id m_binded_shader = 0;
    };
}
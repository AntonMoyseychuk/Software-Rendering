#include "shader_engine_api.hpp"

namespace gl {
    _shader_engine_api::_shader_engine_api()
        : m_shader_engine(_shader_engine::get())
    {
    }

    size_t _shader_engine_api::create_shader(const std::shared_ptr<_shader> &shader) const noexcept {
        return m_shader_engine.create_shader(shader);
    }

    void _shader_engine_api::bind_shader(size_t id) const noexcept {
        m_shader_engine.bind_shader(id);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, const math::quaternion &q) const noexcept {
        m_shader_engine.uniform(uniform_name, q);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, const math::mat4f &mat) const noexcept {
        m_shader_engine.uniform(uniform_name, mat);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, const math::vec4f &vec) const noexcept {
        m_shader_engine.uniform(uniform_name, vec);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, const math::vec3f &vec) const noexcept {
        m_shader_engine.uniform(uniform_name, vec);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, const math::vec2f &vec) const noexcept {
        m_shader_engine.uniform(uniform_name, vec);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, float value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, double value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, int8_t value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, int16_t value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, int32_t value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }

    void _shader_engine_api::uniform(const std::string &uniform_name, int64_t value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }
}
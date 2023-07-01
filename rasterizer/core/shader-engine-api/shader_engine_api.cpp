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
}
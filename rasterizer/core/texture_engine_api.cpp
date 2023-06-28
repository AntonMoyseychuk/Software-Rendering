#include "texture_engine_api.hpp"

namespace gl {
    _texture_engine_api::_texture_engine_api()
        : m_tex_engine(_texture_engine::get())
    {
    }
    
    size_t _texture_engine_api::create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void *data) const noexcept {
        return m_tex_engine.create_texture(width, height, channel_count, data);
    }
    
    void _texture_engine_api::bind_texture(size_t id) const noexcept {
        m_tex_engine.bind_texture(id);
    }
}
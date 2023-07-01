#include "texture_engine.hpp"

#include "math_3d/util.hpp"
#include "core/assert_macro.hpp"

#define _ASSERT_TEXTURE_ID_VALIDITY(container, id) ASSERT(container.find((id)) != container.cend(), "texture engine error", "invalid texture ID")

namespace gl {
    _texture_engine &_texture_engine::get() noexcept {
        static _texture_engine engine;
        return engine;
    }
    
    size_t _texture_engine::create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void *data) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_textures.find(id) != m_textures.cend());

        m_textures[id] = _texture(width, height, channel_count, data);
    
        return id;
    }
    
    void _texture_engine::bind_texture(size_t id) noexcept {
        _ASSERT_TEXTURE_ID_VALIDITY(m_textures, id);
        m_binded_texture = id;
    }
    
    const _texture &_texture_engine::_get_binded_texture() const noexcept {
        _ASSERT_TEXTURE_ID_VALIDITY(m_textures, m_binded_texture);
        return m_textures.at(m_binded_texture);
    }
}
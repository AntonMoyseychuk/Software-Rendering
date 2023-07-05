#include "texture_engine.hpp"

#include "math_3d/util.hpp"
#include "core/assert_macro.hpp"

#define _ASSERT_TEXTURE_ID_VALIDITY(container, id) ASSERT(container.find((id)) != container.cend(), "texture engine error", "invalid texture ID")
#define _ASSERT_TEXTURE_SLOT_VALIDITY(container, slot) ASSERT(container.find((slot)) != container.cend(), "texture engine error", "invalid texture slot number")

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

    void _texture_engine::activate_texture(size_t slot) noexcept {
        _ASSERT_TEXTURE_ID_VALIDITY(m_textures, m_binded_texture);
        m_texture_slots[slot] = m_binded_texture;
    }

    const _texture &_texture_engine::_get_slot(size_t slot) const noexcept {
        _ASSERT_TEXTURE_SLOT_VALIDITY(m_texture_slots, slot);
        return m_textures.at(m_texture_slots.at(slot));
    }
}
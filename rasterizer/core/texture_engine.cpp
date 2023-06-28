#include "texture_engine.hpp"
#include "math_3d/util.hpp"

namespace gl {
    _texture_engine &_texture_engine::get() noexcept {
        static _texture_engine engine;
        return engine;
    }
    
    size_t _texture_engine::create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void *data) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (textures.find(id) != textures.cend());

        textures[id] = _texture(width, height, channel_count, data);
    
        return id;
    }
    
    void _texture_engine::bind_texture(size_t id) noexcept {
        binded_texture = id;
    }
}
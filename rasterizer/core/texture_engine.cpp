#include "texture_engine.hpp"
#include "math_3d/util.hpp"

namespace gl {
    _texture_engine &_texture_engine::get() noexcept {
        static _texture_engine engine;
        return engine;
    }
    
    size_t _texture_engine::create_texture(size_t width, size_t height, texture_format format, const void *data) noexcept {
        const auto cend = textures.cend();
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (textures.find(id) != cend);

        textures[id] = _texture(width, height, format, data);

        return id;
    }
    
    void _texture_engine::bind_texture(size_t texture_id) noexcept {
        ASSERT_TEXTURE_ID_VALIDITY(textures, texture_id);
        binded_texture = texture_id;
    }
}
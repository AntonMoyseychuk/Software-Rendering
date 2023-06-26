#include "shader.hpp"

namespace gl {
    static _texture_engine& texture_engine = _texture_engine::get();

    void _shader::geometry() const noexcept
    {
    }
    
    math::color _shader::texture(const math::vec2f &tex_coord) const noexcept {
        ASSERT(math::between(tex_coord.x, 0.0f, 1.0f) && math::between(tex_coord.y, 0.0f, 1.0f), "shader error", "texture coordinates out of range");

        ASSERT(texture_engine.textures.find(texture_engine.binded_texture) != texture_engine.textures.cend(), "texture engine error", "invalid texture ID");
        const _texture& texture = texture_engine.textures[texture_engine.binded_texture];

        const size_t color_start = size_t(tex_coord.x * texture.width + tex_coord.y * texture.height) * texture.channel_count;
        
        const math::color color {
            texture.data[color_start + 0] / 255.0f,    
            texture.data[color_start + 1] / 255.0f,    
            texture.data[color_start + 2] / 255.0f,    
            texture.channel_count == 4 ? texture.data[color_start + 3] / 255.0f : 1.0f,    
        };

        return color;
    }
}
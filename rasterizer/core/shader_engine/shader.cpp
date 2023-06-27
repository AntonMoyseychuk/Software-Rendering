#include "shader.hpp"

#include "core/texture_engine/texture_engine.hpp"

namespace gl {
    static _texture_engine& texture_engine = _texture_engine::get();

    void _shader::geometry() const noexcept
    {
    }
    
    math::color _shader::texture(const math::vec2f &texcoord) const noexcept {
        using namespace math;
        
        ASSERT_TEXTURE_ID_VALIDITY(texture_engine.textures, texture_engine.binded_texture);
        const _texture& t = texture_engine.textures[texture_engine.binded_texture];

        ASSERT(between(texcoord.x, 0.0f, 1.0f) && between(texcoord.y, 0.0f, 1.0f), "shader error", "invalid texture coordinates");

        const size_t color_start = static_cast<size_t>(texcoord.x * t.width + texcoord.y + t.height) * t.channel_count;
        return color {
            t.data[color_start + 0] / 255.0f,
            t.data[color_start + 1] / 255.0f,
            t.data[color_start + 2] / 255.0f,
            t.channel_count == 4 ? t.data[color_start + 3] / 255.0f : 1.0f,
        };
    }
}
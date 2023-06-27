#include "shader.hpp"

namespace gl {
    void _shader::geometry() const noexcept
    {
    }

    const _texture &_shader::sampler_2D() const noexcept {
        static _texture_engine& engine = _texture_engine::get();

        ASSERT_TEXTURE_ID_VALIDITY(engine.textures, engine.binded_texture);
        return engine.textures[engine.binded_texture];
    }

    math::color _shader::texture(const _texture &texture, const math::vec2f &texcoord) const noexcept {
        using namespace math;

        ASSERT(between(texcoord.x, 0.0f, 1.0f) && between(texcoord.y, 0.0f, 1.0f), "shader error", "invalid texture coordinates");

        const size_t color_start = static_cast<size_t>(texcoord.x * texture.width + texcoord.y + texture.height) * texture.channel_count;
        return color {
            texture.data[color_start + 0] / 255.0f,
            texture.data[color_start + 1] / 255.0f,
            texture.data[color_start + 2] / 255.0f,
            texture.channel_count == 4 ? texture.data[color_start + 3] / 255.0f : 1.0f,
        };
    }
}
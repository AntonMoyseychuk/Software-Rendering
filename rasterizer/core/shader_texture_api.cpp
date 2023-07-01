#include"shader_texture_api.hpp"
#include "texture_engine.hpp"

#include "assert_macro.hpp"

namespace gl {
    const _texture &_shader_texture_api::sampler_2D() const noexcept {
        static _texture_engine& engine = _texture_engine::get();
        return engine._get_binded_texture();
    }
    
    math::color _shader_texture_api::texture(const _texture &texture, const math::vec2f &texcoord) const noexcept {
        using namespace math;

        ASSERT(between(texcoord.x, 0.0f, 1.0f) && between(texcoord.y, 0.0f, 1.0f), "shader error", "invalid texture coordinates");

        const size_t x = texcoord.x * (texture.width - 1), y = (1.0f - texcoord.y) * (texture.height - 1);
        const size_t color_start = (x + y * texture.width) * texture.channel_count;
        return color {
            texture.data[color_start + 0] / 255.0f,
            texture.data[color_start + 1] / 255.0f,
            texture.data[color_start + 2] / 255.0f,
            texture.channel_count == 4 ? texture.data[color_start + 3] / 255.0f : 1.0f,
        };
    }
}
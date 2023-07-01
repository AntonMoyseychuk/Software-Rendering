#pragma once
#include "math_3d/math.hpp"

namespace gl {
    struct _texture;

    class _shader_texture_api {
    public:
        _shader_texture_api() = default;
        virtual ~_shader_texture_api() = default;

    protected:
        const _texture& sampler_2D() const noexcept;
        math::color texture(const _texture& texture, const math::vec2f& texcoord) const noexcept;
    };
}
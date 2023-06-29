#pragma once
#include "uniform_buffer_api.hpp"
#include "texture_engine.hpp"

#include <any>

namespace gl {
    struct _shader : public _uniform_buffer_api<bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, 
        math::vec2f, math::vec3f, math::vec4f, math::quaternion, math::mat4f> 
    {
        friend struct _render_engine;
    
        _shader() = default;
        virtual ~_shader() = default;

        virtual std::any vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel(const std::any& vs_out) const noexcept = 0;

        virtual void geometry() const noexcept;

    protected:
        const _texture& sampler_2D() const noexcept;
        math::color texture(const _texture& texture, const math::vec2f& texcoord) const noexcept;

    protected:
        mutable math::vec4f gl_Position;
    };
}
#pragma once
#include "uniform_buffer_api.hpp"
#include "texture_engine.hpp"

#include <any>

namespace gl {
    struct _shader : public _uniform_buffer_api {
        friend struct _render_engine;
    
        _shader() = default;
        virtual ~_shader() = default;

        virtual std::any vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel(const std::any& vs_out) const noexcept = 0;

        // TODO: virtual void geometry() const noexcept {}

    protected:
        const _texture& sampler_2D() const noexcept;
        math::color texture(const _texture& texture, const math::vec2f& texcoord) const noexcept;

    // protected:
    //     template<typename InType>
    //     void in(InType& var, const std::string& tag) const noexcept {
    //         static _shader_engine& engine = _shader_engine::get();
    //     }

    //     template<typename OutType>
    //     void out(const OutType& var, const std::string& tag) const noexcept {
    //         static _shader_engine& engine = _shader_engine::get();
    //         engine.m_intermediates
    //     }

    protected:
        mutable math::vec4f gl_Position;
    };
}
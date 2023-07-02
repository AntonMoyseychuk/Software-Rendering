#pragma once
#include "core/render-engine-api/render_engine_api.hpp"
#include "core/buffer-engine-api/buffer_engine_api.hpp"
#include "core/shader-engine-api/shader_engine_api.hpp"
#include "core/texture-engine-api/texture_engine_api.hpp"


namespace gl {
    class gl_api final : public _render_engine_api, public _buffer_engine_api, public _shader_engine_api, public _texture_engine_api {
    public:
        gl_api(const gl_api& api) = delete;
        gl_api& operator=(const gl_api& api) = delete;

        static gl_api& get() noexcept;

    private:
        gl_api() noexcept;
    };
}
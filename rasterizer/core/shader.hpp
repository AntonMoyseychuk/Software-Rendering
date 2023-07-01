#pragma once
#include "shader_uniform_api.hpp"
#include "shader_texture_api.hpp"
#include "shader_pipeline_api.hpp"

namespace gl {
    class _shader : public _shader_uniform_api, public _shader_texture_api, public _shader_pipeline_api {
    public:
        friend class _render_engine;
    
        _shader() = default;
        virtual ~_shader() = default;

        virtual void vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel() const noexcept = 0;

        virtual void geometry() const noexcept { /*TODO*/ }

    protected:
        mutable math::vec4f gl_Position;
    };
}
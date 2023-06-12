#pragma once
#include "shader.hpp"


namespace rasterization::gfx {
    struct _abstract_shader {
        friend struct _shader_engine;
        friend struct _render_engine;

        _abstract_shader() = default;
        virtual ~_abstract_shader() = default;

        virtual math::vec4f vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel(const void* vertex) const noexcept = 0;

        virtual void geometry() const noexcept;

    protected:
        const math::mat4f& get_mat4_uniform(const std::string& name) const noexcept;
        const math::vec4f& get_vec4_uniform(const std::string& name) const noexcept;
        const math::vec3f& get_vec3_uniform(const std::string& name) const noexcept;
        const math::vec2f& get_vec2_uniform(const std::string& name) const noexcept;
        float get_float_uniform(const std::string& name) const noexcept;

    private:
        uniform_buffer m_uniforms;
    };
}
#pragma once
#include <unordered_map>
#include <string>

#include "math_3d/math.hpp"


namespace rasterization::gfx {
    struct _uniform_buffer final {
        std::unordered_map<std::string, math::mat4f> mat4_uniforms;
        std::unordered_map<std::string, math::vec4f> vec4_uniforms;
        std::unordered_map<std::string, math::vec3f> vec3_uniforms;
        std::unordered_map<std::string, math::vec2f> vec2_uniforms;

        std::unordered_map<std::string, math::quaternion> quaternion_uniforms;

        std::unordered_map<std::string, float> float_uniforms;
    };

    struct _shader {
        friend struct _shader_engine;
        friend struct _render_engine;

        _shader() = default;
        virtual ~_shader() = default;

        virtual void vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel(const void* vertex) const noexcept = 0;

        virtual void geometry() const noexcept;

    protected:
        const math::quaternion& get_quaternion_uniform(const std::string& name) const noexcept;
        const math::mat4f& get_mat4_uniform(const std::string& name) const noexcept;
        const math::vec4f& get_vec4_uniform(const std::string& name) const noexcept;
        const math::vec3f& get_vec3_uniform(const std::string& name) const noexcept;
        const math::vec2f& get_vec2_uniform(const std::string& name) const noexcept;
        float get_float_uniform(const std::string& name) const noexcept;

        mutable math::vec4f gl_Position;

    private:
        _uniform_buffer m_uniforms;
    };
}
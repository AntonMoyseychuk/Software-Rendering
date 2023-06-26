#pragma once
#include <string>
#include <unordered_map>

#include "math_3d/math.hpp"
#include "core/assert_macro.hpp"

#define ASSERT_UNIFORM_VALIDITY(container, uniform_name) ASSERT(container.find((uniform_name)) != container.cend(), "unifrom buffer error", "invalid uniform name")

namespace gl {
    struct _uniforable {
        friend struct _shader_engine;

        _uniforable() = default;
        virtual ~_uniforable() = default;
    
    protected:
        const math::quaternion& get_quaternion_uniform(const std::string& name) const noexcept;
        const math::mat4f& get_mat4_uniform(const std::string& name) const noexcept;
        const math::vec4f& get_vec4_uniform(const std::string& name) const noexcept;
        const math::vec3f& get_vec3_uniform(const std::string& name) const noexcept;
        const math::vec2f& get_vec2_uniform(const std::string& name) const noexcept;
        float              get_f32_uniform(const std::string& name) const noexcept;
        double             get_f64_uniform(const std::string& name) const noexcept;
        int8_t             get_i8_uniform(const std::string& name) const noexcept;
        int16_t            get_i16_uniform(const std::string& name) const noexcept;
        int32_t            get_i32_uniform(const std::string& name) const noexcept;
        int64_t            get_i64_uniform(const std::string& name) const noexcept;

    private:
        struct uniform_buffer {
            std::unordered_map<std::string, math::mat4f> u_mat4;
            std::unordered_map<std::string, math::vec4f> u_vec4;
            std::unordered_map<std::string, math::vec3f> u_vec3;
            std::unordered_map<std::string, math::vec2f> u_vec2;

            std::unordered_map<std::string, math::quaternion> u_quaternion;

            std::unordered_map<std::string, float  > u_f32;
            std::unordered_map<std::string, double > u_f64;
            std::unordered_map<std::string, int8_t >  u_i8;
            std::unordered_map<std::string, int16_t> u_i16;
            std::unordered_map<std::string, int32_t> u_i32;
            std::unordered_map<std::string, int64_t> u_i64;
        } m_uniforms;
    };
}
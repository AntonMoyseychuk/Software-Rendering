#pragma once
#include <string>
#include <variant>
#include <unordered_map>

#include "math_3d/math.hpp"
#include "assert_macro.hpp"


namespace gl {
    struct _uniform_buffer_api {
        friend struct _shader_engine;

        using uniform_type = std::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, 
            math::vec2f, math::vec3f, math::vec4f, math::quaternion, math::mat4f>;

        _uniform_buffer_api() = default;
        virtual ~_uniform_buffer_api() = default;
    
    protected:
        template <typename Uniform>
        const Uniform& get_uniform(const std::string& tag) const noexcept {
            ASSERT(m_uniforms.u.find((tag)) != m_uniforms.u.cend(), "unifrom buffer error", "invalid uniform tag");
            
            const auto& var = m_uniforms.u.at(tag);
            ASSERT(std::holds_alternative<Uniform>(var), "unifrom buffer error", "invalid Uniform type passed");

            return std::get<Uniform>(var);
        }

    private:
        template <typename Uniform>
        void _set_uniform(const Uniform& uniform, const std::string& tag) noexcept {
            m_uniforms.u[tag] = uniform;
        }

    private:
        struct uniform_buffer {
            std::unordered_map<std::string, uniform_type> u;
        };

        uniform_buffer m_uniforms;
    };
}
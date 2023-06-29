#pragma once
#include <string>
#include <variant>
#include <unordered_map>

#include "math_3d/math.hpp"
#include "assert_macro.hpp"


namespace gl {
    template<typename... Args>
    struct _uniform_buffer_api {
        friend struct _shader_engine;

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

    // private:
    //     template <typename Uniform>
    //     void set_uniform(const Uniform& uniform, const std::string& tag) noexcept {
    //         m_uniforms.u[tag] = uniform;
    //     }

    private:
        template<typename... Args2>
        struct uniform_buffer {
            std::unordered_map<std::string, std::variant<Args2...>> u;
        };

        uniform_buffer<Args...> m_uniforms;
    };
}
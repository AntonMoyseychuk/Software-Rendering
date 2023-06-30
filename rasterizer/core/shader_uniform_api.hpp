#pragma once
#include <string>
#include <variant>
#include <unordered_map>

#include "math_3d/math.hpp"
#include "assert_macro.hpp"

#define _ASSERT_UNIFORM_ID_VALIDITY(container, id) ASSERT(container.find((id)) != container.cend(), "unifrom buffer api error", "invalid uniform tag")
#define _ASSERT_UNIFORM_TYPE_VALIDITY(type, uniform) ASSERT(std::holds_alternative<type>(uniform), "unifrom buffer api error", std::string("invalid Uniform type passed - ") + typeid(Uniform).name())

namespace gl {
    struct _shader_uniform_api {
        friend struct _shader_engine;

        _shader_uniform_api() = default;
        virtual ~_shader_uniform_api() = default;
    
    protected:
        template <typename Uniform>
        const Uniform& get_uniform(const std::string& tag) const noexcept {
            using namespace std;

            _ASSERT_UNIFORM_ID_VALIDITY(m_uniforms, tag);
            
            const uniform_type& var = m_uniforms.at(tag);
            _ASSERT_UNIFORM_TYPE_VALIDITY(Uniform, var);

            return get<Uniform>(var);
        }

    private:
        template <typename Uniform>
        void _set_uniform(const Uniform& uniform, const std::string& tag) noexcept {
            #ifdef _DEBUG
            if (m_uniforms.find(tag) != m_uniforms.cend()) {
                _ASSERT_UNIFORM_TYPE_VALIDITY(Uniform, m_uniforms.at(tag));
            }
            #endif
            m_uniforms[tag] = uniform;
        }

    public:
        using uniform_type = std::variant<int32_t, uint32_t, int64_t, uint64_t, float, double, math::vec2f, math::vec3f, math::vec4f, math::mat4f>;
        using uniform_pack = std::unordered_map<std::string, uniform_type>;

    private:
        uniform_pack m_uniforms;
    };
}
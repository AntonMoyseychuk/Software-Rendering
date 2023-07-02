#pragma once
#include "math_3d/math.hpp"
#include "core/assert_macro.hpp"

#include <unordered_map>
#include <variant>
#include <memory>

namespace gl {
    class _shader;

    class _shader_engine final {
    public:
        _shader_engine(const _shader_engine& engine) = delete;
        _shader_engine& operator=(const _shader_engine& engine) = delete;

        static _shader_engine& get() noexcept;

        size_t create_shader(const std::shared_ptr<_shader>& shader) noexcept;
        void bind_shader(size_t id) noexcept;

        template<typename Uniform>
        void uniform(const Uniform& uniform, const std::string& uniform_tag) noexcept {
            #ifdef _DEBUG
                ASSERT(m_shader_programs.find(m_binded_shader) != m_shader_programs.cend(), "shader engine error", "invalid shader program ID");
                
                #define PACK m_shader_programs.at(m_binded_shader).uniforms
                if (PACK.find(uniform_tag) != PACK.cend()) {
                    ASSERT(std::holds_alternative<Uniform>(PACK.at(uniform_tag)), "shader engine error", "type redefinition of " + uniform_tag);
                }
                #undef PACK
            #endif
            
            m_shader_programs.at(m_binded_shader).uniforms[uniform_tag] = uniform;
        }

    public:
        template<typename Uniform>
        const Uniform& _get_uniform(const std::string& uniform_tag) const noexcept {
            #ifdef _DEBUG
                ASSERT(m_shader_programs.find(m_binded_shader) != m_shader_programs.cend(), "shader engine error", "invalid shader program ID");
                
                #define PACK m_shader_programs.at(m_binded_shader).uniforms
                ASSERT(PACK.find(uniform_tag) != PACK.cend(), "shader engine error", "invalid uniform tag");
                ASSERT(std::holds_alternative<Uniform>(PACK.at(uniform_tag)), "shader engine error", 
                    "\"" + uniform_tag + "\" has different type than " + typeid(Uniform).name());
                #undef PACK
            #endif

            const uniform_type& uniform = m_shader_programs.at(m_binded_shader).uniforms.at(uniform_tag);
            return std::get<Uniform>(uniform);
        }

    private:
        _shader_engine() = default;

    private:
        using uniform_type = std::variant<bool, int32_t, uint32_t, int64_t, uint64_t, float, double, 
            math::vec2f, math::vec3f, math::vec4f, math::mat4f>;
        using uniforms_pack_type = std::unordered_map<std::string, uniform_type>;

        struct shader_program final {
            uniforms_pack_type uniforms;
            std::shared_ptr<_shader> shader;
        };
        using shader_id = size_t;
        
    public:
        const shader_program& _get_binded_shader_program() const noexcept;

    private:
        std::unordered_map<shader_id, shader_program> m_shader_programs;
        shader_id m_binded_shader = 0;
    };
}
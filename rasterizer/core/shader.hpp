#pragma once
#include "shader_uniform_api.hpp"
#include "shader_texture_api.hpp"

#include <unordered_map>
#include <variant>

namespace gl {
    class _shader : public _shader_uniform_api, public _shader_texture_api {
    public:
        friend class _render_engine;
    
        _shader() = default;
        virtual ~_shader() = default;

        virtual void vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel() const noexcept = 0;

        virtual void geometry() const noexcept { /*TODO*/ }

    protected:
        template<typename InType>
        const InType& in(const std::string& tag) const noexcept {
            ASSERT(m_intermediate.find(tag) != m_intermediate.cend(), "shader error", "invalid IN variable tag");
            ASSERT(std::holds_alternative<InType>(m_intermediate[tag]), "shader error", "the IN variable \"" + tag + "\" has different type");
            
            return std::get<InType>(m_intermediate.at(tag));
        }

        template<typename OutType>
        void out(const OutType& var, const std::string& tag) const noexcept {
            #ifdef _DEBUG
                if (m_intermediate.find(tag) != m_intermediate.cend()) {
                    ASSERT(std::holds_alternative<OutType>(m_intermediate[tag]), "shader error", 
                        "redefinition a variable with the name" + tag + " but different type"
                    );
                }
            #endif

            m_intermediate[tag] = var;
        }

    protected:
        mutable math::vec4f gl_Position;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    private:
        // using shader_intermediate_data = std::unordered_map<std::string, uniform_type>;

        mutable std::unordered_map<std::string, std::variant<int32_t, uint32_t, int64_t, uint64_t, float, double, 
            math::vec2f, math::vec3f, math::vec4f, math::mat4f>> m_intermediate;
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    };
}
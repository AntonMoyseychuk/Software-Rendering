#pragma once
#include "shader_uniform_api.hpp"
#include "texture_engine.hpp"

namespace gl {
    struct _shader : public _shader_uniform_api {
        friend struct _render_engine;
    
        _shader() = default;
        virtual ~_shader() = default;

        virtual void vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel() const noexcept = 0;

        // TODO: virtual void geometry() const noexcept {}

    protected:
        const _texture& sampler_2D() const noexcept;
        math::color texture(const _texture& texture, const math::vec2f& texcoord) const noexcept;

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
                        "redefinition a variable with the same name but different types"
                    );
                }
            #endif

            m_intermediate[tag] = var;
        }

    protected:
        mutable math::vec4f gl_Position;

    #if 1
    private:
        using in_out_data = std::unordered_map<std::string, uniform_type>;

        mutable in_out_data m_intermediate;
    #endif
    };
}
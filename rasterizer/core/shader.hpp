#pragma once
#include "uniform_buffer_api.hpp"
#include "texture_engine.hpp"

namespace gl {
    struct _shader : public _uniform_buffer_api {
        friend struct _render_engine;

        struct intermediate_buffer_type {
            std::unordered_map<std::string, uniform_type> buffer;
        };
    
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
            ASSERT(m_intermediate.buffer.find(tag) != m_intermediate.buffer.cend(), "shader error", 
                "invalid IN variable tag");
            ASSERT(std::holds_alternative<InType>(m_intermediate.buffer[tag]), "shader error", 
                ("the IN variable \"" + tag + "\" has different type").c_str());
            
            return std::get<InType>(m_intermediate.buffer.at(tag));
        }

        template<typename OutType>
        void out(const OutType& var, const std::string& tag) const noexcept {
            if (m_intermediate.buffer.find(tag) != m_intermediate.buffer.cend()) {
                ASSERT(std::holds_alternative<OutType>(m_intermediate.buffer[tag]), "shader error", 
                    "redefinition a variable with the same name but different types"
                );
            }

            m_intermediate.buffer[tag] = var;
        }

    protected:
        mutable math::vec4f gl_Position;

    private:
        mutable intermediate_buffer_type m_intermediate;
    };
}
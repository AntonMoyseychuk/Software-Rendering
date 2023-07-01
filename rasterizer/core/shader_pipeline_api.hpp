#pragma once
#include "shader_engine.hpp"

namespace gl {
    class _shader_pipeline_api {
        friend class _render_engine;
        
    public:
        _shader_pipeline_api() = default;

    protected:
        template<typename InType>
        const InType& in(const std::string& tag) const noexcept {
            ASSERT(m_pack.find(tag) != m_pack.cend(), "shader error", "invalid IN variable tag");
            ASSERT(std::holds_alternative<InType>(m_pack[tag]), "shader error", "the IN variable \"" + tag + "\" has different type");
            
            return std::get<InType>(m_pack.at(tag));
        }

        template<typename OutType>
        void out(const OutType& var, const std::string& tag) const noexcept {
            #ifdef _DEBUG
                if (m_pack.find(tag) != m_pack.cend()) {
                    ASSERT(std::holds_alternative<OutType>(m_pack[tag]), "shader error", 
                        "redefinition a variable with the name" + tag + " but different type"
                    );
                }
            #endif

            m_pack[tag] = var;
        }

    private:
        mutable _shader_engine::pipeline_pack_type m_pack;
    };
}
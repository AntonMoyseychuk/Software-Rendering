#pragma once
#include "core/render-engine-api/render_engine.hpp"

namespace gl {
    class _shader_pipeline_api {
    public:
        _shader_pipeline_api() = default;

    protected:
        using pd = _render_engine::pipeline_pack_type;
        
        template<typename InType>
        const InType& in(const std::string& tag, const pd& _pd) const noexcept {
            ASSERT(_pd.find(tag) != _pd.cend(), "shader error", "invalid IN variable tag");
            ASSERT(std::holds_alternative<InType>(_pd[tag]), "shader error", "the IN variable \"" + tag + "\" has different type");
            
            return std::get<InType>(_pd.at(tag));
        }

        template<typename OutType>
        void out(const OutType& var, const std::string& tag, pd& _pd) const noexcept {
            #ifdef _DEBUG
                if (_pd.find(tag) != _pd.cend()) {
                    ASSERT(std::holds_alternative<OutType>(_pd[tag]), "shader error", 
                        "redefinition a variable with the name" + tag + " but different type"
                    );
                }
            #endif

            _pd[tag] = var;
        }
    };
}
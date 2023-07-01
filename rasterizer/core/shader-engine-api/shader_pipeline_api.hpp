#pragma once
#include "shader_engine.hpp"

namespace gl {
    class _shader_pipeline_api {
    public:
        _shader_pipeline_api() = default;

    protected:
        using pipeline_data = _shader_engine::pipeline_pack_type;
        
        template<typename InType>
        const InType& in(const std::string& tag, const pipeline_data& pack) const noexcept {
            ASSERT(pack.find(tag) != pack.cend(), "shader error", "invalid IN variable tag");
            ASSERT(std::holds_alternative<InType>(pack[tag]), "shader error", "the IN variable \"" + tag + "\" has different type");
            
            return std::get<InType>(pack.at(tag));
        }

        template<typename OutType>
        void out(const OutType& var, const std::string& tag, pipeline_data& pack) const noexcept {
            #ifdef _DEBUG
                if (pack.find(tag) != pack.cend()) {
                    ASSERT(std::holds_alternative<OutType>(pack[tag]), "shader error", 
                        "redefinition a variable with the name" + tag + " but different type"
                    );
                }
            #endif

            pack[tag] = var;
        }
    };
}
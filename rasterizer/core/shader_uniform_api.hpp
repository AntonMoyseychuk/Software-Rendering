#pragma once
#include "shader_engine.hpp"
#include <string>


namespace gl {
    class _shader_uniform_api {
    public:
        _shader_uniform_api() = default;
    
    protected:
        template <typename Uniform>
        const Uniform& get_uniform(const std::string& tag) const noexcept {
            static _shader_engine& engine = _shader_engine::get();
            return engine._get_uniform<Uniform>(tag);
        }
    };
}
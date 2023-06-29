#pragma once
#include "shader_engine.hpp"

namespace gl {
    class _shader_engine_api {
    public:
        _shader_engine_api();

        size_t create_shader(const std::shared_ptr<_shader>& shader) const noexcept;
        void bind_shader(size_t id) const noexcept;

        template<typename Uniform>
        void uniform(const Uniform& uniform, const std::string& uniform_tag) noexcept {
            m_shader_engine.uniform(uniform, uniform_tag);
        }
    
    private:
        _shader_engine& m_shader_engine;
    };
}
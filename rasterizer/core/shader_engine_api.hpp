#pragma once
#include "shader_engine.hpp"

namespace gl {
    class _shader_engine_api {
    public:
        _shader_engine_api();

        size_t create_shader(const std::shared_ptr<_shader>& shader) const noexcept;
        void bind_shader(size_t id) const noexcept;

        void uniform(const std::string& uniform_name, const math::quaternion& q) const noexcept;
        void uniform(const std::string& uniform_name, const math::mat4f& mat) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec4f& vec) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec3f& vec) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec2f& vec) const noexcept;
        void uniform(const std::string& uniform_name, float value) const noexcept;
        void uniform(const std::string& uniform_name, double value) const noexcept;
        void uniform(const std::string& uniform_name, int8_t value) const noexcept;
        void uniform(const std::string& uniform_name, int16_t value) const noexcept;
        void uniform(const std::string& uniform_name, int32_t value) const noexcept;
        void uniform(const std::string& uniform_name, int64_t value) const noexcept;
    
    private:
        _shader_engine& m_shader_engine;
    };
}
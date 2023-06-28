#pragma once
#include "shader.hpp"

namespace gl {
    struct _shader_engine final {
        _shader_engine(const _shader_engine& engine) = delete;
        _shader_engine& operator=(const _shader_engine& engine) = delete;

        static _shader_engine& get() noexcept;

        size_t create_shader(const std::shared_ptr<_shader>& shader) noexcept;
        void bind_shader(size_t shader_id) noexcept;

        void uniform(const std::string& uniform_name, const math::quaternion& q) noexcept;
        void uniform(const std::string& uniform_name, const math::mat4f& mat) noexcept;
        void uniform(const std::string& uniform_name, const math::vec4f& vec) noexcept;
        void uniform(const std::string& uniform_name, const math::vec3f& vec) noexcept;
        void uniform(const std::string& uniform_name, const math::vec2f& vec) noexcept;
        void uniform(const std::string& uniform_name, float value) noexcept;
        void uniform(const std::string& uniform_name, double value) noexcept;
        void uniform(const std::string& uniform_name, int8_t value) noexcept;
        void uniform(const std::string& uniform_name, int16_t value) noexcept;
        void uniform(const std::string& uniform_name, int32_t value) noexcept;
        void uniform(const std::string& uniform_name, int64_t value) noexcept;

    private:
        _shader_engine() = default;

    public:
        struct shader_program final {
            std::shared_ptr<_shader> shader;
        };
        const shader_program& _get_binded_shader_program() const noexcept;

        using shader_id = size_t;

    private:
        std::unordered_map<shader_id, shader_program> m_shader_programs;
        shader_id m_binded_shader = 0;
    };
}
#pragma once
#include <string>

#include "shader.hpp"

#define ASSERT_UNIFORM_VALIDITY(container, name) assert(container.count((name)) == 1)

namespace rasterization::gfx {
    struct _shader_engine final {
        _shader_engine(const _shader_engine& engine) = delete;
        _shader_engine& operator=(const _shader_engine& engine) = delete;

        static _shader_engine& get() noexcept;

        size_t create_shader(const std::shared_ptr<shader>& shader) noexcept;
        void bind_shader(size_t shader_id) noexcept;

        void uniform(const std::string& uniform_name, const math::mat4f& mat) noexcept;
        void uniform(const std::string& uniform_name, const math::vec4f& vec) noexcept;
        void uniform(const std::string& uniform_name, const math::vec3f& vec) noexcept;
        void uniform(const std::string& uniform_name, const math::vec2f& vec) noexcept;
        void uniform(const std::string& uniform_name, float value) noexcept;

    private:
        _shader_engine() = default;

        struct _shader_program final {
            std::shared_ptr<shader> shader;
            uniform_buffer uniform_buffer;
        };

        using shader_id = size_t;

    public:
        std::unordered_map<shader_id, _shader_program> shader_programs;
        size_t curr_shader = 0;
    };
}
#pragma once
#include <string>

#include "shader.hpp"
#include "abstract_shader.hpp"

namespace rasterization::gfx {
    struct _shader_engine final {
        _shader_engine(const _shader_engine& engine) = delete;
        _shader_engine& operator=(const _shader_engine& engine) = delete;

        static _shader_engine& get() noexcept;

        size_t create_shader(
            const std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)>& vertex,
            const std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)>& pixel
        ) noexcept;
        void bind_shader(size_t shader_id) noexcept;

        size_t create_abstract_shader(const std::shared_ptr<_abstract_shader>& shader) noexcept;
        void bind_abstract_shader(size_t shader_id) noexcept;

        void uniform(const std::string& uniform_name, const math::mat4f& mat) noexcept;
        void uniform(const std::string& uniform_name, const math::vec4f& vec) noexcept;
        void uniform(const std::string& uniform_name, const math::vec3f& vec) noexcept;
        void uniform(const std::string& uniform_name, const math::vec2f& vec) noexcept;
        void uniform(const std::string& uniform_name, float value) noexcept;

        void abstract_uniform(const std::string& uniform_name, const math::mat4f& mat) noexcept;
        void abstract_uniform(const std::string& uniform_name, const math::vec4f& vec) noexcept;
        void abstract_uniform(const std::string& uniform_name, const math::vec3f& vec) noexcept;
        void abstract_uniform(const std::string& uniform_name, const math::vec2f& vec) noexcept;
        void abstract_uniform(const std::string& uniform_name, float value) noexcept;

    private:
        _shader_engine() = default;

    public:
        struct shader_program final {
            uniform_buffer uniform_buffer;
            _shader shader;

            math::vec4f vs(const void* vertex) const noexcept { return shader.vertex(uniform_buffer, vertex); }
            math::color ps(const void* vertex) const noexcept { return shader.pixel(uniform_buffer, vertex); }
        };

        struct abstract_shader_program final {
            std::shared_ptr<_abstract_shader> shader;
        };

        const shader_program& _get_binded_shader_program() const noexcept;
        const abstract_shader_program& _get_binded_abstract_shader_program() const noexcept;

        using shader_id = size_t;

    public:
        std::unordered_map<shader_id, shader_program> shader_programs;
        std::unordered_map<shader_id, abstract_shader_program> abstract_shader_programs;
        size_t binded_shader = 0;
    };
}
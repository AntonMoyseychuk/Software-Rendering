#include "gl_api.hpp"
#include "math_3d/util.hpp"
#include "math_3d/mat_operations.hpp"

#include <cassert>

namespace rasterization::gfx {
    gl_api &gl_api::get() noexcept {
        static gl_api engine;
        return engine;
    }

    gl_api::gl_api() noexcept 
        : m_buf_engine(_buffer_engine::get()), m_render_engine(_render_engine::get()), m_shader_engine(_shader_engine::get())
    {
    }

    void gl_api::viewport(uint32_t width, uint32_t height) const noexcept {
        m_viewport = math::viewport(width, height);
    }

#pragma region render_engine_api
    bool gl_api::bind_window(win_framewrk::Window *window) noexcept {
        return m_render_engine.bind_window(window);
    }

    const win_framewrk::Window *gl_api::is_window_binded() const noexcept {
        return m_render_engine.is_window_binded();
    }

    void gl_api::render(render_mode mode) const noexcept {
        m_render_engine.render(mode);
    }

    void gl_api::swap_buffers() const noexcept {
        m_render_engine.swap_buffers();
    }

    void gl_api::clear_depth_buffer() const noexcept {
        m_render_engine.clear_depth_buffer();
    }

    void gl_api::set_clear_color(const math::color &color) noexcept {
        m_render_engine.set_clear_color(color);
    }
#pragma endregion render_engine_api

#pragma region buffer_engine_api
    size_t gl_api::create_vertex_buffer(const void* buffer, size_t size) const noexcept {
        return m_buf_engine.create_vertex_buffer(buffer, size);
    }

    size_t gl_api::create_index_buffer(const size_t *buffer, size_t count) const noexcept {
        return m_buf_engine.create_index_buffer(buffer, count);
    }

    void gl_api::delete_vertex_buffer(size_t id) const noexcept {
        m_buf_engine.delete_vertex_buffer(id);
    }

    void gl_api::delete_index_buffer(size_t id) const noexcept {
        m_buf_engine.delete_index_buffer(id);
    }

    void gl_api::bind_buffer(buffer_type type, size_t id) const noexcept {
        m_buf_engine.bind_buffer(type, id);
    }

    void gl_api::set_buffer_element_size(size_t size) const noexcept {
        m_buf_engine.set_buffer_element_size(size);
    }
#pragma endregion buffer_engine_api

#pragma region shader_engine_api
    size_t gl_api::create_shader(const std::shared_ptr<_shader> &shader) const noexcept {
        return m_shader_engine.create_shader(shader);
    }

    void gl_api::bind_shader(size_t shader_id) const noexcept {
        m_shader_engine.bind_shader(shader_id);
    }

    void gl_api::uniform(const std::string &uniform_name, const math::quaternion &q) const noexcept {
        m_shader_engine.uniform(uniform_name, q);
    }

    void gl_api::uniform(const std::string &uniform_name, const math::mat4f &mat) const noexcept {
        m_shader_engine.uniform(uniform_name, mat);
    }

    void gl_api::uniform(const std::string &uniform_name, const math::vec4f &vec) const noexcept {
        m_shader_engine.uniform(uniform_name, vec);
    }

    void gl_api::uniform(const std::string &uniform_name, const math::vec3f &vec) const noexcept {
        m_shader_engine.uniform(uniform_name, vec);
    }

    void gl_api::uniform(const std::string &uniform_name, const math::vec2f &vec) const noexcept {
        m_shader_engine.uniform(uniform_name, vec);
    }

    void gl_api::uniform(const std::string &uniform_name, float value) const noexcept {
        m_shader_engine.uniform(uniform_name, value);
    }
#pragma endregion shader_engine_api
}
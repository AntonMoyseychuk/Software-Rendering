#include "gl_api.hpp"
#include "math_3d/util.hpp"
#include "math_3d/mat_operations.hpp"

#include <cassert>

namespace rasterization::gfx {
    gl_api &gl_api::get() noexcept {
        static gl_api engine;
        return engine;
    }

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

    void gl_api::clear_backbuffer() const noexcept {
        m_render_engine.clear_backbuffer();
    }

    void gl_api::set_clear_color(const math::color &color) noexcept {
        m_render_engine.set_clear_color(color);
    }

    gl_api::gl_api() noexcept 
        : m_buf_engine(_buffer_engine::get()), m_render_engine(_render_engine::get())
    {
    }

    const std::vector<uint8_t> &gl_api::_get_binded_vertex_buffer() const noexcept {
        assert(m_buf_engine.m_vbos.count(m_buf_engine.m_curr_vbo) > 0);
        return m_buf_engine.m_vbos[m_buf_engine.m_curr_vbo].data;
    }

    const std::vector<size_t> &gl_api::_get_binded_index_buffer() const noexcept {
        assert(m_buf_engine.m_ibos.count(m_buf_engine.m_curr_ibo) > 0);
        return m_buf_engine.m_ibos[m_buf_engine.m_curr_ibo];
    }

#pragma region buffer_engine_api
    size_t gl_api::create_vertex_buffer(const void* buffer, size_t size) const noexcept {
        return m_buf_engine.create_vertex_buffer(buffer, size);
    }

    void gl_api::vertex_attrib_pointer(size_t vbo_id, size_t size, attrib_data_type type, size_t stride, const void *ptr) const noexcept {
        m_buf_engine.vertex_attrib_pointer(vbo_id, size, type, stride, ptr);
    }

    size_t gl_api::create_index_buffer(const size_t *buffer, size_t count) const noexcept {
        return m_buf_engine.create_index_buffer(buffer, count);
    }

    void gl_api::erase_buffer(size_t id) const noexcept {
        m_buf_engine.erase_buffer(id);
    }

    void gl_api::erase_index_buffer(size_t id) const noexcept {
        m_buf_engine.erase_index_buffer(id);
    }

    void gl_api::bind(buffer_type type, size_t id) const noexcept {
        m_buf_engine.bind(type, id);
    }
#pragma endregion buffer_engine_api

    void gl_api::uniform(const std::string& uniform_name, const math::mat4f &mat) noexcept {
        m_mat4_uniforms[uniform_name] = mat;
    }

    void gl_api::uniform(const std::string &uniform_name, const math::vec4f &vec) noexcept {
        m_vec4f_uniforms[uniform_name] = vec;
    }

    void gl_api::uniform(const std::string &uniform_name, const math::vec3f &vec) noexcept {
        m_vec3f_uniforms[uniform_name] = vec;
    }

    void gl_api::uniform(const std::string &uniform_name, const math::vec2f &vec) noexcept {
        m_vec2f_uniforms[uniform_name] = vec;
    }

    void gl_api::uniform(const std::string &uniform_name, float value) noexcept {
        m_float_uniforms[uniform_name] = value;
    }

    void gl_api::viewport(uint32_t width, uint32_t height) noexcept {
        m_viewport = math::viewport(width, height);
    }
}
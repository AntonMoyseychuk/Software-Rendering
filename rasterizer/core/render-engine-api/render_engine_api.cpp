#include "render_engine_api.hpp"

namespace gl {
    _render_engine_api::_render_engine_api()
        : m_render_engine(_render_engine::get())
    {
    }

    bool _render_engine_api::bind_window(win_framewrk::Window *window) const noexcept {
        return m_render_engine.bind_window(window);
    }

    const win_framewrk::Window *_render_engine_api::is_window_binded() const noexcept {
        return m_render_engine.is_window_binded();
    }

    void _render_engine_api::render(render_mode mode) const noexcept {
        m_render_engine.render(mode);
    }

    void _render_engine_api::swap_buffers() const noexcept {
        m_render_engine.swap_buffers();
    }

    void _render_engine_api::clear_depth_buffer() const noexcept {
        m_render_engine.clear_depth_buffer();
    }

    void _render_engine_api::set_clear_color(const math::color &color) const noexcept {
        m_render_engine.set_clear_color(color);
    }

    void _render_engine_api::viewport(uint32_t width, uint32_t height) const noexcept {
        m_render_engine.viewport(width, height);
    }
}
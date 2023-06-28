#pragma once
#include "render_engine.hpp"

namespace gl {
    class _render_engine_api {
    public:
        _render_engine_api();

        bool bind_window(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* is_window_binded() const noexcept;

        void render(render_mode mode) const noexcept;
        void swap_buffers() const noexcept;
        void clear_depth_buffer() const noexcept;

        void set_clear_color(const math::color& color) noexcept;

        void viewport(uint32_t width, uint32_t height) const noexcept;
    
    private:
        _render_engine& m_render_engine;
    };
}
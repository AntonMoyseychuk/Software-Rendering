#pragma once
#include "window/window.hpp"

#include "math_3d/vec4.hpp"


namespace rasterization::gfx {
    enum class render_mode : uint8_t { POINTS, LINES, LINE_STRIP, TRIANGLES };

    struct _render_engine final {
        _render_engine(const _render_engine& engine) = delete;
        _render_engine& operator=(const _render_engine& engine) = delete;

        static _render_engine& get() noexcept;
        
        bool bind_window(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* is_window_binded() const noexcept;

        void render(render_mode mode) const noexcept;
        void swap_buffers() const noexcept;
        void clear_backbuffer() const noexcept;

        void set_clear_color(const math::color& color) noexcept;

    private:
        _render_engine() noexcept = default;

        void _resize_z_buffer(uint32_t width, uint32_t height) const noexcept;
        bool _check_and_update_depth(const math::vec3f& pixel) const noexcept;

        void _rasterize(const std::vector<math::vec3f> &screen_coords, std::vector<math::vec3f> &raster_coords) const noexcept;

        void _render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept;
        void _render_line(const math::vec3f& pixel_0, const math::vec3f& pixel_1, const math::color& color) const noexcept;
        void _render_triangle(const math::vec3f &pixel_0, const math::vec3f &pixel_1, const math::vec3f &pixel_2, const math::color& color) const noexcept;

    private:
        mutable std::vector<float> m_z_buffer;

        win_framewrk::Window* m_window_ptr = nullptr;
        math::color m_clear_color = math::color::BLACK;
    };
}
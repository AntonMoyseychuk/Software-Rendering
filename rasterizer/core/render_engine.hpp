#pragma once
#include "math_3d/math.hpp"
#include "window/window.hpp"
#include "thread_pool/thread_pool.hpp"

#include "shader_engine.hpp"

namespace gl {
    enum class render_mode : uint8_t { POINTS, LINES, LINE_STRIP, TRIANGLES };

    struct _render_engine final {
        _render_engine(const _render_engine& engine) = delete;
        _render_engine& operator=(const _render_engine& engine) = delete;

        static _render_engine& get() noexcept;
        
        bool bind_window(win_framewrk::Window* window) const noexcept;
        const win_framewrk::Window* is_window_binded() const noexcept;

        void viewport(uint32_t width, uint32_t height) const noexcept;

        void render(render_mode mode) const noexcept;
        void swap_buffers() const noexcept;
        void clear_depth_buffer() const noexcept;

        void set_clear_color(const math::color& color) noexcept;

    private:
        _render_engine() noexcept;

    private:
        void _resize_z_buffer(uint32_t width, uint32_t height) const noexcept;
        bool _test_and_update_depth(const math::vec3f& pixel) const noexcept;

    private:
        static bool _is_inside_clipping_space(const math::vec3f& point) noexcept;
        static bool _is_back_face(const math::vec3f& v0, const math::vec3f& v1, const math::vec3f& v2) noexcept;
        
        /**
         * returns:
         *  > 0: point is to the right of the edge
         *    0: point is on the edge
         *  < 0: point is to the left of the edge
        */
        static double _edge(const math::vec2f& v0, const math::vec2f& v1, const math::vec2f& p) noexcept;

    private:
        struct intermediate_data {
            intermediate_data() = default;
            intermediate_data(const _shader::intermediate_buffer_type& data, const math::vec4f& coord, bool clipped) 
                : clipped(clipped), data(data), coord(coord) {}

            bool clipped;
            _shader::intermediate_buffer_type data;
            math::vec4f coord;
        };
        
        void _render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept;

        void _render_line(const intermediate_data& v0, const intermediate_data& v1) const noexcept;
        void _render_line_low(const intermediate_data& v0, const intermediate_data& v1) const noexcept;
        void _render_line_high(const intermediate_data& v0, const intermediate_data& v1) const noexcept;

        void _render_polygon(const intermediate_data& v0, const intermediate_data& v1, const intermediate_data& v2) const noexcept;

    private:
        mutable std::vector<float> m_z_buffer;
        mutable std::vector<intermediate_data> m_vs_intermediates;

        mutable util::ThreadPool m_thread_pool = { 1 };

        mutable math::mat4f m_viewport;

        mutable win_framewrk::Window* m_window_ptr = nullptr;
        math::color m_clear_color = math::color::BLACK;
    };
}
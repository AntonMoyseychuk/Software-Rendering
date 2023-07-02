#pragma once
#include "math_3d/math.hpp"
#include "window/window.hpp"
#include "thread_pool/thread_pool.hpp"

#include <unordered_map>
#include <variant>

namespace gl {
    enum class render_mode : uint8_t { POINTS, LINES, LINE_STRIP, TRIANGLES };

    class _render_engine final {
    public:
        using pipeline_data_type = std::variant<math::vec2f, math::vec3f, math::vec4f>;
        using pipeline_pack_type = std::unordered_map<std::string, pipeline_data_type>;

        _render_engine(const _render_engine& engine) = delete;
        _render_engine& operator=(const _render_engine& engine) = delete;

        static _render_engine& get() noexcept;
        
        bool bind_window(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* is_window_binded() const noexcept;

        void viewport(uint32_t width, uint32_t height) noexcept;

        void render(render_mode mode) noexcept;
        void swap_buffers() noexcept;
        void clear_depth_buffer() noexcept;

        void set_clear_color(const math::color& color) noexcept;

    private:
        _render_engine() noexcept;

    private:
        void _resize_z_buffer(uint32_t width, uint32_t height) noexcept;
        bool _test_and_update_depth(const math::vec3f& pixel) noexcept;

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
        struct pipeline_metadata {
            bool clipped = false;
            pipeline_pack_type in_out_data;
            math::vec4f coord;
        };
        
        void _render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept;

        void _render_line(const pipeline_metadata& v0, const pipeline_metadata& v1) const noexcept;
        void _render_line_low(const pipeline_metadata& v0, const pipeline_metadata& v1) const noexcept;
        void _render_line_high(const pipeline_metadata& v0, const pipeline_metadata& v1) const noexcept;

        void _render_polygon(const pipeline_metadata& v0, const pipeline_metadata& v1, const pipeline_metadata& v2) noexcept;

    private:
        static pipeline_data_type _interpolate2(float w0, float w1, const pipeline_data_type& var0, const pipeline_data_type& var1) noexcept;
        static pipeline_data_type _interpolate3(float w0, float w1, float w2, 
            const pipeline_data_type& var0, const pipeline_data_type& var1, const pipeline_data_type& var2) noexcept;

    private:
        std::vector<float> m_z_buffer;
        std::vector<pipeline_metadata> m_pipeline_data;

        util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };

        math::mat4f m_viewport;

        win_framewrk::Window* m_window_ptr = nullptr;
        math::color m_clear_color = math::color::BLACK;
    };
}
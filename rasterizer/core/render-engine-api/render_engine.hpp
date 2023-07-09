#pragma once
#include "window/window.hpp"
#include "thread_pool/thread_pool.hpp"

#include "math_3d/math.hpp"
#include "core/assert_macro.hpp"

#include <unordered_map>
#include <variant>

namespace gl {
    enum class render_mode : uint8_t { POINTS, LINES, LINE_STRIP, TRIANGLES };

    class _render_engine final {
    public:
        using pipeline_data_type = std::variant<math::vec2f, math::vec3f, math::vec4f, math::mat4f>;
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
        template <size_t N>
        struct barycentric_interpolator {
            static_assert(math::between(N, static_cast<size_t>(2), static_cast<size_t>(4)));
        };

        template<>
        struct barycentric_interpolator<3> {
            barycentric_interpolator(const math::vec3d& w, const pipeline_data_type& v0, const pipeline_data_type& v1) 
                : w(w), v0(v0), v1(v1) {}

            pipeline_data_type operator()(const math::vec2f& v2) const noexcept {
                ASSERT(std::holds_alternative<math::vec2f>(v0), "render engine error", "type of v0 is not \'vec2f\'");
                ASSERT(std::holds_alternative<math::vec2f>(v1), "render engine error", "type of v1 is not \'vec2f\'");

                return std::get<math::vec2f>(v0) * w.x + std::get<math::vec2f>(v1) * w.y + v2 * w.z;
            }

            pipeline_data_type operator()(const math::vec3f& v2) const noexcept {
                ASSERT(std::holds_alternative<math::vec3f>(v0), "render engine error", "type of v0 is not \'vec3f\'");
                ASSERT(std::holds_alternative<math::vec3f>(v1), "render engine error", "type of v1 is not \'vec3f\'");

                return std::get<math::vec3f>(v0) * w.x + std::get<math::vec3f>(v1) * w.y + v2 * w.z;
            }

            pipeline_data_type operator()(const math::vec4f& v2) const noexcept {
                ASSERT(std::holds_alternative<math::vec4f>(v0), "render engine error", "type of v0 is not \'vec4f\'");
                ASSERT(std::holds_alternative<math::vec4f>(v1), "render engine error", "type of v1 is not \'vec4f\'");

                return std::get<math::vec4f>(v0) * w.x + std::get<math::vec4f>(v1) * w.y + v2 * w.z;
            }

            pipeline_data_type operator()(const math::mat4f& v2) const noexcept {
                ASSERT(std::holds_alternative<math::mat4f>(v0), "render engine error", "type of v0 is not \'mat4f\'");
                ASSERT(std::holds_alternative<math::mat4f>(v1), "render engine error", "type of v1 is not \'mat4f\'");

                return v2;
            }

            math::vec3d w;
            const pipeline_data_type &v0, &v1;
        };

        template<>
        struct barycentric_interpolator<2> {
            barycentric_interpolator(const math::vec2d& w, const pipeline_data_type& v0) 
                : w(w), v0(v0) {}

            pipeline_data_type operator()(const math::vec2f& v1) const noexcept {
                ASSERT(std::holds_alternative<math::vec2f>(v0), "render engine error", "type of v0 is not \'vec2f\'");

                return std::get<math::vec2f>(v0) * w.x + v1 * w.y;
            }

            pipeline_data_type operator()(const math::vec3f& v1) const noexcept {
                ASSERT(std::holds_alternative<math::vec3f>(v0), "render engine error", "type of v0 is not \'vec3f\'");

                return std::get<math::vec3f>(v0) * w.x + v1 * w.y;
            }

            pipeline_data_type operator()(const math::vec4f& v1) const noexcept {
                ASSERT(std::holds_alternative<math::vec4f>(v0), "render engine error", "type of v0 is not \'vec4f\'");

                return std::get<math::vec4f>(v0) * w.x + v1 * w.y;
            }

            pipeline_data_type operator()(const math::mat4f& v1) const noexcept {
                ASSERT(std::holds_alternative<math::mat4f>(v0), "render engine error", "type of v0 is not \'mat4f\'");

                return v1;
            }

            math::vec2d w;
            const pipeline_data_type &v0;
        };

    private:
        std::vector<float> m_z_buffer;
        std::vector<pipeline_metadata> m_pipeline_data;

        util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };

        math::mat4f m_viewport;

        win_framewrk::Window* m_window_ptr = nullptr;
        math::color m_clear_color = math::color::BLACK;
    };
}
#include "render_engine.hpp"

#include "core/gl_api.hpp"

#include "core/buffer_engine/buffer_engine.hpp"
#include "core/shader_engine/shader_engine.hpp"

#include "core/assert_macro.hpp"
#include "math_3d/util.hpp"

namespace gl {
    static gl_api& core = gl_api::get();
    static _buffer_engine& buff_engine = _buffer_engine::get();
    static _shader_engine& shader_engine = _shader_engine::get();

    _render_engine::_render_engine() noexcept 
    {
    }

    void _render_engine::render(render_mode mode) const noexcept {
        using namespace math;

    #pragma region input-assembler    
        const _buffer_engine::vertex_buffer& vbo = buff_engine._get_binded_vertex_buffer();
        const _buffer_engine::index_buffer& ibo = buff_engine._get_binded_index_buffer();
    #pragma endregion input-assembler

    #pragma region resizing-buffers
        const size_t vertex_count = vbo.data.size() / vbo.element_size;

        m_vs_intermediates.resize(vertex_count);
        _resize_z_buffer(m_window_ptr->GetWidth(), m_window_ptr->GetHeight());
    #pragma endregion resizing-buffers

    #pragma region local-to-raster-coords
        const _shader_engine::shader_program& shader_program = shader_engine._get_binded_shader_program();

        for (size_t i = 0, j = 0; j < vertex_count; i += vbo.element_size, ++j) {
            m_vs_intermediates[j].vs_out = shader_program.shader->vertex(&vbo.data[i]);
            m_vs_intermediates[j].coord = shader_program.shader->gl_Position;
            
            const vec4f ndc = m_vs_intermediates[j].coord.xyz / m_vs_intermediates[j].coord.w;
            m_vs_intermediates[j].clipped = !_is_inside_clipping_space(ndc.xyz);

            m_vs_intermediates[j].coord = ndc * core.m_viewport;
            m_vs_intermediates[j].coord.x = std::floor(m_vs_intermediates[j].coord.x);
            m_vs_intermediates[j].coord.y = std::floor(m_vs_intermediates[j].coord.y);
        }
    #pragma endregion local-to-raster-coords

        switch (mode) {
        case render_mode::POINTS:
            for (size_t i = 0; i < vertex_count; ++i) {
                if (!m_vs_intermediates[i].clipped) {
                    _render_pixel(m_vs_intermediates[i].coord.xy, shader_program.shader->pixel(m_vs_intermediates[i].vs_out));
                }
            }    
            break;
        
        case render_mode::LINES:
            for (size_t i = 1; i < ibo.data.size(); i += 2) {
                if (!m_vs_intermediates[ibo.data[i - 1]].clipped && !m_vs_intermediates[ibo.data[i]].clipped) {
                    _render_line(m_vs_intermediates[ibo.data[i - 1]], m_vs_intermediates[ibo.data[i]]);
                }
            }
            break;
        
        case render_mode::LINE_STRIP: 
            for (size_t i = 1; i < ibo.data.size(); ++i) {
                if (!m_vs_intermediates[ibo.data[i - 1]].clipped && !m_vs_intermediates[ibo.data[i]].clipped) {
                    _render_line(m_vs_intermediates[ibo.data[i - 1]], m_vs_intermediates[ibo.data[i]]);
                }
            }
            break;

        case render_mode::TRIANGLES:
            for (size_t i = 2; i < ibo.data.size(); i += 3) {
                if (!m_vs_intermediates[ibo.data[i - 2]].clipped && !m_vs_intermediates[ibo.data[i - 1]].clipped && !m_vs_intermediates[ibo.data[i]].clipped) {
                    const vec3f& v0 = m_vs_intermediates[ibo.data[i - 2]].coord.xyz;
                    const vec3f& v1 = m_vs_intermediates[ibo.data[i - 1]].coord.xyz; 
                    const vec3f& v2 = m_vs_intermediates[ibo.data[i - 0]].coord.xyz;
                    if (!_is_back_face(v0, v1, v2)) {
                        m_thread_pool.AddTask(&_render_engine::_render_triangle, this, 
                            std::cref(m_vs_intermediates[ibo.data[i - 2]]), 
                            std::cref(m_vs_intermediates[ibo.data[i - 1]]), 
                            std::cref(m_vs_intermediates[ibo.data[i - 0]])
                        );
                    }
                }
            }

            m_thread_pool.WaitAll();
            break;

        default:
            ASSERT(false, "runtime", "invalid Rendering Mode");
            break;
        }
    }

    void _render_engine::_render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept {
        m_window_ptr->SetPixelColor(pixel.x, pixel.y, R_G_B_A(color));
    }

    void _render_engine::_render_line(const vs_intermediate_data& v0, const vs_intermediate_data& v1) const noexcept {
        if (math::abs(v1.coord.y - v0.coord.y) < math::abs(v1.coord.x - v0.coord.x)) {
            (v1.coord.x < v0.coord.x) ? _render_line_low(v1, v0) :  _render_line_low(v0, v1);
        } else {
            (v1.coord.y < v0.coord.y) ? _render_line_high(v1, v0) : _render_line_high(v0, v1);
        }
    }

    void _render_engine::_render_line_low(const vs_intermediate_data& v0, const vs_intermediate_data& v1) const noexcept {
        using namespace math;

        const auto& shader = shader_engine._get_binded_shader_program().shader;
        
        float dx = v1.coord.x - v0.coord.x;
        float dy = v1.coord.y - v0.coord.y;

        float yi = 1.0f;

        if (dy < 0.0f) {
            yi = -1.0f;
            dy = -dy;
        }

        const color c0 = shader->pixel(v0.vs_out), c1 = shader->pixel(v1.vs_out);
        const float v0_v1_dist = (v1.coord.xy - v0.coord.xy).length();

        float D = (2.0f * dy) - dx;
        float y = v0.coord.y;

        for (float x = v0.coord.x; x <= v1.coord.x; ++x) {
            const vec2f pixel(x, y);
            const float w0 = (pixel - v0.coord.xy).length() / v0_v1_dist;
            const float w1 = 1.0f - w0;
            _render_pixel(pixel, c0 * w1 + c1 * w0);

            if (D > 0) {
                y += yi;
                D += 2.0f * (dy - dx);
            } else {
                D += 2.0f * dy;
            }
        }
    }

    void _render_engine::_render_line_high(const vs_intermediate_data& v0, const vs_intermediate_data& v1) const noexcept {
        using namespace math;

        const auto& shader = shader_engine._get_binded_shader_program().shader;
        
        float dx = v1.coord.x - v0.coord.x;
        float dy = v1.coord.y - v0.coord.y;

        float xi = 1.0f;

        if (dx < 0.0f) {
            xi = -1.0f;
            dx = -dx;
        }

        const color c0 = shader->pixel(v0.vs_out), c1 = shader->pixel(v1.vs_out);
        const float v0_v1_dist = (v1.coord.xy - v0.coord.xy).length();

        float D = (2.0f * dx) - dy;
        float x = v0.coord.x;

        for (float y = v0.coord.y; y <= v1.coord.y; ++y) {
            const vec2f pixel(x, y);
            const float w0 = (pixel - v0.coord.xy).length() / v0_v1_dist;
            const float w1 = 1.0f - w0;
            _render_pixel(pixel, c0 * w1 + c1 * w0);

            if (D > 0) {
                x += xi;
                D += 2.0f * (dx - dy);
            } else {
                D += 2.0f * dx;
            }
        }
    }

    void _render_engine::_render_triangle(const vs_intermediate_data& v0, const vs_intermediate_data& v1, const vs_intermediate_data& v2) const noexcept {
        using namespace math;
        using namespace std;

        const auto& shader = shader_engine._get_binded_shader_program().shader;

        const vec2i bboxmin(round(min(min(v0.coord.x, v1.coord.x), v2.coord.x)), round(min(min(v0.coord.y, v1.coord.y), v2.coord.y)));
        const vec2i bboxmax(round(max(max(v0.coord.x, v1.coord.x), v2.coord.x)), round(max(max(v0.coord.y, v1.coord.y), v2.coord.y)));

        const color c0 = shader->pixel(v0.vs_out), c1 = shader->pixel(v1.vs_out), c2 = shader->pixel(v2.vs_out);

        for (float y = bboxmin.y; y <= bboxmax.y; ++y) {
            bool prev_pixel_was_inside = false;
            for (float x = bboxmin.x; x <= bboxmax.x; ++x) {
                const vec2f p(x, y);
                
                const double area = _edge(v0.coord.xy, v1.coord.xy, v2.coord.xy);
                const double   w0 = _edge(v1.coord.xy, v2.coord.xy, p) / area;
                const double   w1 = _edge(v2.coord.xy, v0.coord.xy, p) / area;
                const double   w2 = 1.0 - w0 - w1;
                
                if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                    prev_pixel_was_inside = true;

                    const double invert_z = (1.0 / v0.coord.z) * w0 + (1.0 / v1.coord.z) * w1 + (1.0 / v2.coord.z) * w2;
                    if (_test_and_update_depth(vec3f(p, 1.0 / invert_z))) {
                        _render_pixel(p, w0 * c0 + w1 * c1 + w2 * c2);
                    }
                } else if (prev_pixel_was_inside) {
                    break;
                }
            }
        }
    }

    void _render_engine::_resize_z_buffer(uint32_t width, uint32_t height) const noexcept {
        m_z_buffer.resize(width * height, std::numeric_limits<float>::max());
    }

    bool _render_engine::_test_and_update_depth(const math::vec3f& pixel) const noexcept {
        const size_t idx = std::round(pixel.x + pixel.y * m_window_ptr->GetWidth());
        if (pixel.z <= m_z_buffer[idx]) {
            m_z_buffer[idx] = pixel.z;
            return true; 
        }

        return false;
    }

    bool _render_engine::_is_inside_clipping_space(const math::vec3f &point) noexcept {
        return math::abs(point.x) <= 1.0f && math::abs(point.y) <= 1.0f && point.z <= -1.0f;
    }

    bool _render_engine::_is_back_face(const math::vec3f &v0, const math::vec3f &v1, const math::vec3f &v2) noexcept {
        static const math::vec3f& backward = math::vec3f::BACKWARD();
        
        const math::vec3f n = normalize(cross(v1 - v0, v2 - v0));
        return dot(n, backward) <= 0.0f;
    }

    double _render_engine::_edge(const math::vec2f &v0, const math::vec2f &v1, const math::vec2f &p) noexcept {
        return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
    }

    _render_engine &_render_engine::get() noexcept {
        static _render_engine renderer;
        return renderer;
    }

    bool _render_engine::bind_window(win_framewrk::Window *window) noexcept
    {
        if (window == nullptr) {
            return false;
        }

        m_window_ptr = window;
        _resize_z_buffer(window->GetWidth(), window->GetHeight());
        return true;
    }

    const win_framewrk::Window *_render_engine::is_window_binded() const noexcept {
        return m_window_ptr;
    }

    void _render_engine::swap_buffers() const noexcept {
        m_window_ptr->PresentPixelBuffer();
        m_window_ptr->FillPixelBuffer(R_G_B_A(m_clear_color));
    }

    void _render_engine::clear_depth_buffer() const noexcept {
        std::fill(m_z_buffer.begin(), m_z_buffer.end(), std::numeric_limits<float>::max());
    }

    void _render_engine::set_clear_color(const math::color& color) noexcept {
        m_clear_color = color;
    }
}
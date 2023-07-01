#include "render_engine.hpp"
#include "buffer_engine.hpp"

#include "assert_macro.hpp"

namespace gl {
    static _buffer_engine& buff_engine = _buffer_engine::get();
    static _shader_engine& shader_engine = _shader_engine::get();

    _render_engine::_render_engine() noexcept 
    {
    }

    void _render_engine::render(render_mode mode) noexcept {
        using namespace math;

    #pragma region input-assembler    
        const _buffer_engine::vertex_buffer& vbo = buff_engine._get_binded_vertex_buffer();
        const _buffer_engine::index_buffer& ibo = buff_engine._get_binded_index_buffer();
    #pragma endregion input-assembler

    #pragma region resizing-buffers
        const size_t vertex_count = vbo.data.size() / vbo.element_size;

        m_shader_data.resize(vertex_count);
        _resize_z_buffer(m_window_ptr->GetWidth(), m_window_ptr->GetHeight());
    #pragma endregion resizing-buffers

    #pragma region local-to-raster-coords
        const auto shader_ptr = shader_engine._get_binded_shader_program().shader;
        
        for (size_t i = 0, j = 0; j < vertex_count; i += vbo.element_size, ++j) {
            shader_ptr->vertex(&vbo.data[i]);
            m_shader_data[j].data = shader_ptr->m_intermediate;
            m_shader_data[j].coord = shader_ptr->gl_Position;
            
            const vec4f ndc = m_shader_data[j].coord.xyz / m_shader_data[j].coord.w;
            m_shader_data[j].clipped = !_is_inside_clipping_space(ndc.xyz);

            m_shader_data[j].coord = ndc * m_viewport;
            m_shader_data[j].coord.x = std::floor(m_shader_data[j].coord.x);
            m_shader_data[j].coord.y = std::floor(m_shader_data[j].coord.y);
        }
    #pragma endregion local-to-raster-coords

        switch (mode) {
        case render_mode::POINTS:
            for (size_t i = 0; i < vertex_count; ++i) {
                if (!m_shader_data[i].clipped) {
                    shader_ptr->m_intermediate = m_shader_data[i].data;
                    _render_pixel(m_shader_data[i].coord.xy, shader_ptr->pixel());
                }
            }    
            break;
        
        case render_mode::LINES:
            for (size_t i = 1; i < ibo.data.size(); i += 2) {
                if (!m_shader_data[ibo.data[i - 1]].clipped && !m_shader_data[ibo.data[i]].clipped) {
                    _render_line(m_shader_data[ibo.data[i - 1]], m_shader_data[ibo.data[i]]);
                }
            }
            break;
        
        case render_mode::LINE_STRIP: 
            for (size_t i = 1; i < ibo.data.size(); ++i) {
                if (!m_shader_data[ibo.data[i - 1]].clipped && !m_shader_data[ibo.data[i]].clipped) {
                    _render_line(m_shader_data[ibo.data[i - 1]], m_shader_data[ibo.data[i]]);
                }
            }
            break;

        case render_mode::TRIANGLES:
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            for (size_t i = 2; i < ibo.data.size(); i += 3) {
                const auto& data0 = m_shader_data[ibo.data[i - 2]], &data1 = m_shader_data[ibo.data[i - 1]], &data2 = m_shader_data[ibo.data[i]];
                if (!data0.clipped && !data1.clipped && !data2.clipped) {
                    if (!_is_back_face(data0.coord.xyz, data1.coord.xyz, data2.coord.xyz)) {
                        m_thread_pool.Wait(m_thread_pool.AddTask(&_render_engine::_render_polygon, this, std::cref(data0), std::cref(data1), std::cref(data2)));
                    }
                }
            }

            // m_thread_pool.WaitAll();
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            break;

        default:
            ASSERT(false, "runtime", "invalid Rendering Mode");
            break;
        }
    }

    void _render_engine::_render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept {
        m_window_ptr->SetPixelColor(pixel.x, pixel.y, R_G_B_A(color));
    }

    void _render_engine::_render_line(const shader_data& v0, const shader_data& v1) const noexcept {
        if (math::abs(v1.coord.y - v0.coord.y) < math::abs(v1.coord.x - v0.coord.x)) {
            (v1.coord.x < v0.coord.x) ? _render_line_low(v1, v0) :  _render_line_low(v0, v1);
        } else {
            (v1.coord.y < v0.coord.y) ? _render_line_high(v1, v0) : _render_line_high(v0, v1);
        }
    }

    void _render_engine::_render_line_low(const shader_data& v0, const shader_data& v1) const noexcept {
        using namespace math;

        const auto& shader = shader_engine._get_binded_shader_program().shader;
        
        float dx = v1.coord.x - v0.coord.x;
        float dy = v1.coord.y - v0.coord.y;

        float yi = 1.0f;

        if (dy < 0.0f) {
            yi = -1.0f;
            dy = -dy;
        }

        const float v0_v1_dist = (v1.coord.xy - v0.coord.xy).length();

        float D = (2.0f * dy) - dx;
        float y = v0.coord.y;

        for (float x = v0.coord.x; x <= v1.coord.x; ++x) {
            const vec2f pixel(x, y);
            
            const float w0 = (pixel - v0.coord.xy).length() / v0_v1_dist;
            const float w1 = 1.0f - w0;

            /////////////////////////////////////////////////////////////////////////////////////////////////////
            _shader::shader_intermediate_data intermediate;
            for (const auto& node : v0.data) {
                if (std::holds_alternative<vec2f>(node.second)) {
                    const vec2f& vec0 = std::get<vec2f>(node.second);
                    const vec2f& vec1 = std::get<vec2f>(v1.data.at(node.first));
                    intermediate[node.first] = vec0 * w1 + vec1 * w0;
                } else if (std::holds_alternative<vec3f>(node.second)) {
                    const vec3f& vec0 = std::get<vec3f>(node.second);
                    const vec3f& vec1 = std::get<vec3f>(v1.data.at(node.first));
                    intermediate[node.first] = vec0 * w1 + vec1 * w0;
                } else if (std::holds_alternative<vec4f>(node.second)) {
                    const vec4f& vec0 = std::get<vec4f>(node.second);
                    const vec4f& vec1 = std::get<vec4f>(v1.data.at(node.first));
                    intermediate[node.first] = vec0 * w1 + vec1 * w0;
                } else {
                    intermediate[node.first] = node.second;
                }
            }

            shader->m_intermediate = intermediate;
            _render_pixel(pixel, shader->pixel());
            /////////////////////////////////////////////////////////////////////////////////////////////////////

            if (D > 0) {
                y += yi;
                D += 2.0f * (dy - dx);
            } else {
                D += 2.0f * dy;
            }
        }
    }

    void _render_engine::_render_line_high(const shader_data& v0, const shader_data& v1) const noexcept {
        using namespace math;

        const auto& shader = shader_engine._get_binded_shader_program().shader;
        
        float dx = v1.coord.x - v0.coord.x;
        float dy = v1.coord.y - v0.coord.y;

        float xi = 1.0f;

        if (dx < 0.0f) {
            xi = -1.0f;
            dx = -dx;
        }

        const float v0_v1_dist = (v1.coord.xy - v0.coord.xy).length();

        float D = (2.0f * dx) - dy;
        float x = v0.coord.x;

        for (float y = v0.coord.y; y <= v1.coord.y; ++y) {
            const vec2f pixel(x, y);
            const float w0 = (pixel - v0.coord.xy).length() / v0_v1_dist;
            const float w1 = 1.0f - w0;

            /////////////////////////////////////////////////////////////////////////////////////////////////////
            _shader::shader_intermediate_data intermediate;
            for (const auto& node : v0.data) {
                if (std::holds_alternative<vec2f>(node.second)) {
                    const vec2f& vec0 = std::get<vec2f>(node.second);
                    const vec2f& vec1 = std::get<vec2f>(v1.data.at(node.first));
                    intermediate[node.first] = vec0 * w1 + vec1 * w0;
                } else if (std::holds_alternative<vec3f>(node.second)) {
                    const vec3f& vec0 = std::get<vec3f>(node.second);
                    const vec3f& vec1 = std::get<vec3f>(v1.data.at(node.first));
                    intermediate[node.first] = vec0 * w1 + vec1 * w0;
                } else if (std::holds_alternative<vec4f>(node.second)) {
                    const vec4f& vec0 = std::get<vec4f>(node.second);
                    const vec4f& vec1 = std::get<vec4f>(v1.data.at(node.first));
                    intermediate[node.first] = vec0 * w1 + vec1 * w0;
                } else {
                    intermediate[node.first] = node.second;
                }
            }

            shader->m_intermediate = intermediate;
            _render_pixel(pixel, shader->pixel());
            /////////////////////////////////////////////////////////////////////////////////////////////////////

            if (D > 0) {
                x += xi;
                D += 2.0f * (dx - dy);
            } else {
                D += 2.0f * dx;
            }
        }
    }

    void _render_engine::_render_polygon(const shader_data& v0, const shader_data& v1, const shader_data& v2) noexcept {
        using namespace math;
        using namespace std;

        const auto& shader = shader_engine._get_binded_shader_program().shader;

        const vec2f bboxmin(round(min(min(v0.coord.x, v1.coord.x), v2.coord.x)), round(min(min(v0.coord.y, v1.coord.y), v2.coord.y)));
        const vec2f bboxmax(round(max(max(v0.coord.x, v1.coord.x), v2.coord.x)), round(max(max(v0.coord.y, v1.coord.y), v2.coord.y)));

        for (float y = bboxmin.y; y <= bboxmax.y; ++y) {
            bool prev_pixel_was_inside = false;
            
            for (float x = bboxmin.x; x <= bboxmax.x; ++x) {
                vec3f pixel(x, y, 0.0);
                
                const double area = _edge(v0.coord.xy, v1.coord.xy, v2.coord.xy);
                const double   w0 = _edge(v1.coord.xy, v2.coord.xy, pixel.xy) / area;
                const double   w1 = _edge(v2.coord.xy, v0.coord.xy, pixel.xy) / area;
                const double   w2 = 1.0 - w0 - w1;
                
                if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                    prev_pixel_was_inside = true;

                    pixel.z = 1.0f / ((1.0f / v0.coord.z) * w0 + (1.0f / v1.coord.z) * w1 + (1.0f / v2.coord.z) * w2);
                    if (_test_and_update_depth(pixel)) {
                        /////////////////////////////////////////////////////////////////////////////////////////////////////
                        _shader::shader_intermediate_data intermediate;
                        for (const auto& node : v0.data) {
                            if (std::holds_alternative<vec2f>(node.second)) {
                                const vec2f& vec0 = std::get<vec2f>(node.second);
                                const vec2f& vec1 = std::get<vec2f>(v1.data.at(node.first));
                                const vec2f& vec2 = std::get<vec2f>(v2.data.at(node.first));
                                intermediate[node.first] = vec0 * w0 + vec1 * w1 + vec2 * w2;
                            } else if (std::holds_alternative<vec3f>(node.second)) {
                                const vec3f& vec0 = std::get<vec3f>(node.second);
                                const vec3f& vec1 = std::get<vec3f>(v1.data.at(node.first));
                                const vec3f& vec2 = std::get<vec3f>(v2.data.at(node.first));
                                intermediate[node.first] = vec0 * w0 + vec1 * w1 + vec2 * w2;
                            } else if (std::holds_alternative<vec4f>(node.second)) {
                                const vec4f& vec0 = std::get<vec4f>(node.second);
                                const vec4f& vec1 = std::get<vec4f>(v1.data.at(node.first));
                                const vec4f& vec2 = std::get<vec4f>(v2.data.at(node.first));
                                intermediate[node.first] = vec0 * w0 + vec1 * w1 + vec2 * w2;
                            } else {
                                intermediate[node.first] = node.second;
                            }
                        }

                        shader->m_intermediate = intermediate;
                        _render_pixel(pixel.xy, shader->pixel());
                        /////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                } else if (prev_pixel_was_inside) {
                    break;
                }
            }
        }
    }

    void _render_engine::_resize_z_buffer(uint32_t width, uint32_t height) noexcept {
        m_z_buffer.resize(width * height, math::MATH_INFINITY);
    }

    bool _render_engine::_test_and_update_depth(const math::vec3f& pixel) noexcept {
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

    bool _render_engine::bind_window(win_framewrk::Window *window) noexcept {
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

    void _render_engine::viewport(uint32_t width, uint32_t height) noexcept {
        m_viewport = math::viewport(width, height);
    }

    void _render_engine::swap_buffers() noexcept {
        m_window_ptr->PresentPixelBuffer();
        m_window_ptr->FillPixelBuffer(R_G_B_A(m_clear_color));
    }

    void _render_engine::clear_depth_buffer() noexcept {
        std::fill(m_z_buffer.begin(), m_z_buffer.end(), math::MATH_INFINITY);
    }

    void _render_engine::set_clear_color(const math::color& color) noexcept {
        m_clear_color = color;
    }
}
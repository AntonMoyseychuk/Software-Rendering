#include "render_engine.hpp"

#include "core/gl_api.hpp"

#include "core/buffer_engine.hpp"

#include "core/shader_engine.hpp"
#include "core/shader_engine_macros.hpp"

#include "math_3d/vec_operations.hpp"
#include "math_3d/mat_operations.hpp"

#include "math_3d/util.hpp"

#include <cassert>

namespace rasterization::gfx {
    static gl_api& core = gl_api::get();
    static _buffer_engine& buff_engine = _buffer_engine::get();
    static _shader_engine& shader_engine = _shader_engine::get();

    void _render_engine::render(render_mode mode) const noexcept {
        using namespace math;

    #pragma region input-assembler    
        const auto& vbo = buff_engine._get_binded_vertex_buffer();
        const auto& ibo = buff_engine._get_binded_index_buffer();
        const std::vector<uint8_t>& local_coords = vbo.data;
        const std::vector<size_t>& indexes = ibo.data;
    #pragma endregion input-assembler

    #pragma region resizing-buffers
        const size_t vertex_count = local_coords.size();

        static std::vector<vec4f> screen_coords;
        static std::vector<vec3f> raster_coords;
        static std::vector<bool> inside_clipping_space;

        screen_coords.resize(vertex_count);
        raster_coords.resize(vertex_count);
        inside_clipping_space.resize(vertex_count);
        _resize_z_buffer(m_window_ptr->GetWidth(), m_window_ptr->GetHeight());
    #pragma endregion resizing-buffers

    #pragma region local-to-raster-coords
        const auto& shader_program = shader_engine._get_binded_shader_program();
        for (size_t i = 0, j = 0; i < local_coords.size(); i += vbo.element_size, ++j) {
            screen_coords[j] = shader_program.shader->vertex(&local_coords[i]);

            const vec3f ndc = screen_coords[j].xyz / screen_coords[j].w;
            inside_clipping_space[j] = _is_inside_clipping_space(ndc);

            raster_coords[j] = ndc * core.m_viewport;
            raster_coords[j].x = std::floor(raster_coords[j].x);
            raster_coords[j].y = std::floor(raster_coords[j].y);
        }
    #pragma endregion local-to-raster-coords

        // Pixel Shaders
        switch (mode) {
        case render_mode::POINTS: {
            const color point_color = shader_program.shader->get_vec4_uniform("point_color");
            for (size_t i = 0; i < indexes.size(); ++i) {
                if (inside_clipping_space[indexes[i]]) {
                    _render_pixel(raster_coords[indexes[i]].xy, point_color);
                }
            }    
            break;
        }
        
        case render_mode::LINES: {
            assert(false && "render_mode::LINES is not imlemented");
            break;
            // ASSERT_UNIFORM_VALIDITY(shader_program._uniform_buffer.vec4_uniforms, "line_color");
            //
            // for (size_t i = 1; i < indexes.size(); i += 2) {
            //     if (is_inside_clipping_space(ndc_coords[indexes[i - 1]]) && is_inside_clipping_space(ndc_coords[indexes[i]])) {
            //         _render_line(raster_coords[indexes[i - 1]], raster_coords[indexes[i]], shader_program._uniform_buffer.vec4_uniforms.at("line_color"));
            //     }
            // }
            // break;
        }
        
        case render_mode::LINE_STRIP: {
            assert(false && "render_mode::LINE_STRIP is not imlemented");
            break;
            // ASSERT_UNIFORM_VALIDITY(shader_program._uniform_buffer.vec4_uniforms, "line_color");
            //
            // for (size_t i = 1; i < indexes.size(); ++i) {
            //     if (is_inside_clipping_space(ndc_coords[indexes[i - 1]]) && is_inside_clipping_space(ndc_coords[indexes[i]])) {
            //         _render_line(raster_coords[indexes[i - 1]], raster_coords[indexes[i]], shader_program._uniform_buffer.vec4_uniforms.at("line_color"));
            //     }
            // }
            // break;
        }

        case render_mode::TRIANGLES: {
            const color polygon_color = shader_program.shader->get_vec4_uniform("polygon_color");
            for (size_t i = 2; i < indexes.size(); i += 3) {
                if (inside_clipping_space[indexes[i - 2]] && inside_clipping_space[indexes[i - 1]] && inside_clipping_space[indexes[i]]) {
                    const vec3f normal = normalize(cross(
                        screen_coords[indexes[i]].xyz - screen_coords[indexes[i - 2]].xyz,
                        screen_coords[indexes[i - 1]].xyz - screen_coords[indexes[i]].xyz
                    ));
                    
                    const float intensity = dot(normal, shader_program.shader->get_vec3_uniform("light_dir")) + 0.1f;
                    _render_triangle(raster_coords[indexes[i - 2]], raster_coords[indexes[i - 1]], raster_coords[indexes[i]], polygon_color * intensity);
                }
            }
            break;
        }

        default:
            assert(false && "Invalid Rendering Mode");
            break;
        }
    }

    void _render_engine::_render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept {
        m_window_ptr->SetPixelColor(pixel.x, pixel.y, R_G_B_A(color));
    }

    void _render_engine::_render_line(const math::vec3f& pix0, const math::vec3f& pix1, const math::color& color) const noexcept {
        using namespace math;

        auto v0 = pix0;
        auto v1 = pix1;

        static std::vector<int32_t> values;
        static std::vector<float> z_values;

        if (abs(v1.x - v0.x) > abs(v1.y - v0.y)) {
            if (v0.x > v1.x) {
                std::swap(v0, v1);
            }

            interpolate<int32_t>(v0.x, v0.y, v1.x, v1.y, values);
            interpolate<float>(v0.x, v0.z, v1.x, v1.z, z_values);
            for (int32_t x = v0.x; x <= v1.x; ++x) {
                const vec3f pixel(x, values[x - v0.x], z_values[x - v0.x]);
                if (_test_and_update_depth(pixel)) {
                    _render_pixel(pixel.xy, color);
                }
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            interpolate<int32_t>(v0.y, v0.x, v1.y, v1.x, values);
            interpolate<float>(v0.y, v0.z, v1.y, v1.z, z_values);
            for (int32_t y = v0.y; y <= v1.y; ++y) {
                const vec3f pixel(values[y - v0.y], y, z_values[y - v0.y]);
                if (_test_and_update_depth(pixel)) {
                    _render_pixel(pixel.xy, color);
                }
            }
        }
    }

    void _render_engine::_render_triangle(const math::vec3f &pix0, const math::vec3f &pix1, const math::vec3f &pix2, const math::color& color) const noexcept {
        using namespace math;

        const vec2i bboxmin(std::min(std::min(pix0.x, pix1.x), pix2.x), std::min(std::min(pix0.y, pix1.y), pix2.y));
        const vec2i bboxmax(std::max(std::max(pix0.x, pix1.x), pix2.x), std::max(std::max(pix0.y, pix1.y), pix2.y));

        for (size_t y = bboxmin.y; y <= bboxmax.y; ++y) {
            for (size_t x = bboxmin.x; x <= bboxmax.x; ++x) {
                const vec2f p(x, y);
                
                const float area = _edge(pix0.xy, pix1.xy, pix2.xy);
                const float w0 = _edge(pix1.xy, pix2.xy, p) / area;
                const float w1 = _edge(pix2.xy, pix0.xy, p) / area;
                const float w2 = 1.0f - w0 - w1;
                
                if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                    const float depth = pix0.z * w0 + pix1.z * w1 + pix2.z * w2;
                    if (_test_and_update_depth(math::vec3f(p, depth))) {
                        _render_pixel(p, color);
                    }
                }
            }
        }
    }

    void _render_engine::_resize_z_buffer(uint32_t width, uint32_t height) const noexcept {
        m_z_buffer.resize(width * height, std::numeric_limits<float>::max());
    }

    bool _render_engine::_test_and_update_depth(const math::vec3f& pixel) const noexcept {
        const int64_t idx = pixel.x + pixel.y * m_window_ptr->GetWidth();
        if (math::between<size_t>(idx, 0, m_z_buffer.size())) {
            if (pixel.z <= m_z_buffer[idx]) {
                m_z_buffer[static_cast<size_t>(idx)] = pixel.z;
                return true; 
            }
        }

        return false;
    }

    bool _render_engine::_is_inside_clipping_space(const math::vec3f &point) noexcept {
        return math::abs(point.x) <= 1.0f && math::abs(point.y) <= 1.0f && point.z <= -1.0f;
    }

    float _render_engine::_edge(const math::vec2f &v0, const math::vec2f &v1, const math::vec2f &p) noexcept {
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

    void _render_engine::clear_backbuffer() const noexcept {
        std::fill(m_z_buffer.begin(), m_z_buffer.end(), std::numeric_limits<float>::max());
    }

    void _render_engine::set_clear_color(const math::color& color) noexcept {
        m_clear_color = color;
    }
}
#include "render_engine.hpp"
#include "render_engine_macros.hpp"

#include "core/gl_api.hpp"
#include "core/buffer_engine.hpp"
#include "core/shader_engine.hpp"
#include "core/shader_engine_macros.hpp"

#include "math_3d/vec_operations.hpp"
#include "math_3d/mat_operations.hpp"

#include "math_3d/util.hpp"


namespace rasterization::gfx {
    bool is_inside_clipping_space(const math::vec3f& point) noexcept {
        return math::abs(point.x) < 1.0f && math::abs(point.y) < 1.0f;
    }

    static gl_api& core = gl_api::get();
    static _buffer_engine& buff_engine = _buffer_engine::get();
    static _shader_engine& shader_engine = _shader_engine::get();

    void _render_engine::render(render_mode mode) const noexcept {
        using namespace math;

    #pragma region input-assembler
        ASSERT_BUFFER_VALIDITY(buff_engine.vbos, buff_engine.curr_vbo);
        ASSERT_BUFFER_VALIDITY(buff_engine.ibos, buff_engine.curr_ibo);
        
        const auto& vbo = buff_engine.vbos[buff_engine.curr_vbo];
        const auto& ibo = buff_engine.ibos[buff_engine.curr_ibo];
        const std::vector<uint8_t>& local_coords = vbo.data;
        const std::vector<size_t>& indexes = ibo.data;
    #pragma endregion input-assembler

    #pragma region resizing-buffers
        const size_t vertex_count = local_coords.size();

        static std::vector<vec4f> screen_coords;
        static std::vector<vec3f> ndc_coords;
        static std::vector<vec3f> raster_coords;

        screen_coords.resize(vertex_count);
        ndc_coords.resize(vertex_count);
        raster_coords.resize(vertex_count);
        _resize_z_buffer(m_window_ptr->GetWidth(), m_window_ptr->GetHeight());
    #pragma endregion resizing-buffers

    #pragma region VS
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_engine.shader_programs, shader_engine.curr_shader);

        const auto& shader_program = shader_engine.shader_programs[shader_engine.curr_shader];
        for (size_t i = 0, j = 0; i < local_coords.size(); i += vbo.element_size, ++j) {
            screen_coords[j] = shader_program.shader.vertex(shader_program.uniform_buffer, &local_coords[i]);
        }
    #pragma endregion VS

    #pragma region NDC
        for (size_t i = 0; i < vertex_count; ++i) {
            ndc_coords[i] = screen_coords[i].xyz / screen_coords[i].w;
        }
    #pragma endregion NDC

        _rasterize(ndc_coords, raster_coords);

        // Pixel Shaders
        switch (mode) {
        case render_mode::POINTS:
            assert(false && "render_mode::POINTS is not imlemented");
            break;
            // ASSERT_UNIFORM_VALIDITY(shader_program.uniform_buffer.vec4f_uniforms, "point_color");
            //
            // for (size_t i = 0; i < indexes.size(); ++i) {
            //     if (is_inside_clipping_space(ndc_coords[indexes[i]])) {
            //         _render_pixel(raster_coords[indexes[i]], shader_program.uniform_buffer.vec4f_uniforms.at("point_color"));
            //     }
            // }    
            // break;
        
        case render_mode::LINES:
            assert(false && "render_mode::LINES is not imlemented");
            break;
            // ASSERT_UNIFORM_VALIDITY(shader_program.uniform_buffer.vec4f_uniforms, "line_color");
            //
            // for (size_t i = 1; i < indexes.size(); i += 2) {
            //     if (is_inside_clipping_space(ndc_coords[indexes[i - 1]]) && is_inside_clipping_space(ndc_coords[indexes[i]])) {
            //         _render_line(raster_coords[indexes[i - 1]], raster_coords[indexes[i]], shader_program.uniform_buffer.vec4f_uniforms.at("line_color"));
            //     }
            // }
            // break;
        
        case render_mode::LINE_STRIP:
            assert(false && "render_mode::LINE_STRIP is not imlemented");
            break;
            // ASSERT_UNIFORM_VALIDITY(shader_program.uniform_buffer.vec4f_uniforms, "line_color");
            //
            // for (size_t i = 1; i < indexes.size(); ++i) {
            //     if (is_inside_clipping_space(ndc_coords[indexes[i - 1]]) && is_inside_clipping_space(ndc_coords[indexes[i]])) {
            //         _render_line(raster_coords[indexes[i - 1]], raster_coords[indexes[i]], shader_program.uniform_buffer.vec4f_uniforms.at("line_color"));
            //     }
            // }
            // break;

        case render_mode::TRIANGLES:
            ASSERT_UNIFORM_VALIDITY(shader_program.uniform_buffer.vec3f_uniforms, "light_dir");
            ASSERT_UNIFORM_VALIDITY(shader_program.uniform_buffer.vec4f_uniforms, "polygon_color");
            
            for (size_t i = 0; i < indexes.size(); i += 3) {
                if (is_inside_clipping_space(ndc_coords[indexes[i]]) && 
                    is_inside_clipping_space(ndc_coords[indexes[i + 1]]) && 
                    is_inside_clipping_space(ndc_coords[indexes[i + 2]])
                ) {
                    const vec3f normal = normalize(cross(
                        screen_coords[indexes[i + 2]].xyz - screen_coords[indexes[i]].xyz,
                        screen_coords[indexes[i + 1]].xyz - screen_coords[indexes[i]].xyz
                    ));

                    const float light_intensity = dot(normal, shader_program.uniform_buffer.vec3f_uniforms.at("light_dir")) + 0.1f;
                    const color color = shader_program.uniform_buffer.vec4f_uniforms.at("polygon_color") * light_intensity;

                    _render_triangle(raster_coords[indexes[i]], raster_coords[indexes[i + 1]], raster_coords[indexes[i + 2]], color);
                }
            }
            break;

        default:
            assert(false && "Invalid Rendering Mode");
            break;
        }
    }

    void _render_engine::_rasterize(const std::vector<math::vec3f> &screen_coords, std::vector<math::vec3f> &raster_coords) const noexcept {
        for (size_t i = 0; i < screen_coords.size(); ++i) {
            raster_coords[i] = screen_coords[i] * core.m_viewport;
            raster_coords[i].x = std::floor(raster_coords[i].x);
            raster_coords[i].y = std::floor(raster_coords[i].y);
        }
    }

    void _render_engine::_render_pixel(const math::vec3f& pixel, const math::color& color) const noexcept {
        m_window_ptr->SetPixelColor(pixel.x, pixel.y, R_G_B_A(color));
    }

    void _render_engine::_render_line(const math::vec3f& raster_coord_v0, const math::vec3f& raster_coord_v1, const math::color& color) const noexcept {
        using namespace math;

        auto v0 = raster_coord_v0;
        auto v1 = raster_coord_v1;

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
                if (_check_and_update_depth(pixel)) {
                    _render_pixel(pixel, color);
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
                if (_check_and_update_depth(pixel)) {
                    _render_pixel(pixel, color);
                }
            }
        }
    }

    void _render_engine::_render_triangle(
        const math::vec3f &raster_coord_0, 
        const math::vec3f &raster_coord_1, 
        const math::vec3f &raster_coord_2, 
        const math::color& color
    ) const noexcept {
        using namespace math;

        auto v0 = raster_coord_0, v1 = raster_coord_1, v2 = raster_coord_2;
        if (v0.y > v1.y) { std::swap(v0, v1); }
        if (v0.y > v2.y) { std::swap(v0, v2); }
        if (v1.y > v2.y) { std::swap(v1, v2); }

        static std::vector<float> z_values; 

        const uint32_t total_height = v2.y - v0.y;
        for (uint32_t i = 0; i < total_height; ++i) {
            const bool is_second_half = i > v1.y - v0.y || v1.y == v0.y;
            const uint32_t segment_height = is_second_half ? v2.y - v1.y : v1.y - v0.y;
            
            const float alpha = static_cast<float>(i) / total_height;
            const float beta  = static_cast<float>(i - (is_second_half ? v1.y - v0.y : 0)) / segment_height;
            
            vec3f left = v0 + (v2 - v0) * alpha;
            vec3f right = is_second_half ? v1 + (v2 - v1) * beta : v0 + (v1 - v0) * beta;
            if (left.x > right.x) { std::swap(left, right); }
            
            interpolate<float>(left.x, left.z, right.x, right.z, z_values);

            for (int32_t j = left.x; j <= right.x; ++j) {
                const vec3f pixel(j, v0.y + i, z_values[j - left.x]);
                if (_check_and_update_depth(pixel)) {
                    _render_pixel(pixel, color);
                }
            }
        }
    }

    void _render_engine::_resize_z_buffer(uint32_t width, uint32_t height) const noexcept {
        m_z_buffer.resize(width * height, std::numeric_limits<float>::max());
    }

    bool _render_engine::_check_and_update_depth(const math::vec3f& pixel) const noexcept {
        const int64_t idx = pixel.x + pixel.y * m_window_ptr->GetWidth();
        if (math::between<size_t>(idx, 0, m_z_buffer.size())) {
            if (pixel.z <= m_z_buffer[idx]) {
                m_z_buffer[static_cast<size_t>(idx)] = pixel.z;
                return true; 
            }
        }

        return false;
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
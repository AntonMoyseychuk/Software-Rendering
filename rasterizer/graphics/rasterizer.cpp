#include "rasterizer.hpp"

#include "math_3d/vec_operations.hpp"
#include "math_3d/mat_operations.hpp"

#include "math_3d/util.hpp"

#include <cassert>

namespace rasterization::gfx {
    Rasterizer::Rasterizer()
        : m_core(gl_api::get())
    {
    }

    Rasterizer::Rasterizer(win_framewrk::Window *window)
        : m_window_ptr(window), m_core(gl_api::get())
    {
        BindWindow(window);
    }

    void Rasterizer::Render(RenderMode mode) const noexcept {
        using namespace math;

        // const auto& local_coords = *(std::vector<vec3f>*)&m_core.m_vbos[vbo_id].data;
        const auto& local_coords = *(std::vector<vec3f>*)&m_core._get_vertex_buffer();
        // const auto& indexes = m_core.m_ibos.at(ibo_id);
        const auto& indexes = m_core._get_index_buffer();

        const size_t vertex_count = local_coords.size();

        static std::vector<vec4f> screen_coords;
        static std::vector<vec3f> ndc_coords;
        static std::vector<vec3f> raster_coords;

    #pragma region resizing-buffers
        screen_coords.resize(vertex_count);
        ndc_coords.resize(vertex_count);
        raster_coords.resize(vertex_count);
        _ResizeZBuffer(m_window_ptr->GetWidth(), m_window_ptr->GetHeight());
    #pragma endregion resizing-buffers

        // Vertex Shaders
        for (size_t i = 0; i < vertex_count; ++i) {
            screen_coords[i] = _VertexShader(local_coords[i]);
        }

        // To NDC
        for (size_t i = 0; i < vertex_count; ++i) {
            ndc_coords[i] = screen_coords[i].xyz / screen_coords[i].w;
        }

        // Rasterization
        _Rasterize(ndc_coords, raster_coords);

        // Pixel Shaders
        switch (mode) {
        case RenderMode::POINTS:
            assert(m_core.m_vec4f_uniforms.count("point_color") == 1);

            for (size_t i = 0; i < indexes.size(); ++i) {
                _RenderPixel(raster_coords[indexes[i]], m_core.m_vec4f_uniforms["point_color"]);
            }    
            break;

        case RenderMode::LINES:
            for (size_t i = 0; i < indexes.size(); i += 2) {
                _RenderLine(raster_coords[indexes[i]], raster_coords[indexes[i + 1]]);
            }
            break;

        case RenderMode::TRIANGLES:
            assert(m_core.m_vec3f_uniforms.count("light_dir") == 1);
            
            for (size_t i = 0; i < indexes.size(); i += 3) {
                const vec3f normal = normalize(cross(
                    screen_coords[indexes[i + 2]].xyz - screen_coords[indexes[i]].xyz,
                    screen_coords[indexes[i + 1]].xyz - screen_coords[indexes[i]].xyz
                ));

                const float light_intensity = dot(normal, m_core.m_vec3f_uniforms["light_dir"]) + 0.1f;
                m_core.uniform("light_intensity", light_intensity);

                _RenderTriangle(raster_coords[indexes[i]], raster_coords[indexes[i + 1]], raster_coords[indexes[i + 2]]);
            }
            break;
        }
    }

    math::vec4f Rasterizer::_VertexShader(const math::vec3f& local_coord) const noexcept {
        using namespace math;

        assert(m_core.m_mat4_uniforms.count("model") == 1);
        assert(m_core.m_mat4_uniforms.count("view") == 1);
        assert(m_core.m_mat4_uniforms.count("projection") == 1);
        
        return local_coord * m_core.m_mat4_uniforms["model"] * m_core.m_mat4_uniforms["view"] * m_core.m_mat4_uniforms["projection"];
    }

    void Rasterizer::_Rasterize(const std::vector<math::vec3f> &screen_coords, std::vector<math::vec3f> &raster_coords) const noexcept {
        for (size_t i = 0; i < screen_coords.size(); ++i) {
            raster_coords[i] = screen_coords[i] * m_core.m_viewport;
            raster_coords[i].x = std::floor(raster_coords[i].x);
            raster_coords[i].y = std::floor(raster_coords[i].y);
        }
    }

    void Rasterizer::_RenderPixel(const math::vec3f& raster_coord, const math::color& color) const noexcept {
        const int64_t idx = raster_coord.x + raster_coord.y * m_window_ptr->GetWidth();
        if (idx >= 0 && idx < m_z_buffer.size()) {
            if (raster_coord.z <= m_z_buffer[static_cast<size_t>(idx)]) {
                m_z_buffer[static_cast<size_t>(idx)] = raster_coord.z;
                m_window_ptr->SetPixelColor(raster_coord.x, raster_coord.y, R_G_B_A(color));
            }
        }
    }

    void Rasterizer::_RenderLine(const math::vec3f& raster_coord_v0, const math::vec3f& raster_coord_v1) const noexcept {
        using namespace math;
        
        assert(m_core.m_vec4f_uniforms.count("line_color") == 1);
        const math::color& color = m_core.m_vec4f_uniforms["line_color"];

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
                _RenderPixel(vec3f(x, values[x - v0.x], z_values[x - v0.x]), color);
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            interpolate<int32_t>(v0.y, v0.x, v1.y, v1.x, values);
            interpolate<float>(v0.y, v0.z, v1.y, v1.z, z_values);
            for (int32_t y = v0.y; y <= v1.y; ++y) {
                _RenderPixel(vec3f(values[y - v0.y], y, z_values[y - v0.y]), color);
            }
        }
    }

    void Rasterizer::_RenderTriangle(const math::vec3f &raster_coord_0, const math::vec3f &raster_coord_1, const math::vec3f &raster_coord_2) const noexcept {
        using namespace math;

        assert(m_core.m_vec4f_uniforms.count("polygon_color") == 1);
        assert(m_core.m_float_uniforms.count("light_intensity") == 1);
        const color color = m_core.m_vec4f_uniforms["polygon_color"] * m_core.m_float_uniforms["light_intensity"];

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
                _RenderPixel(vec3f(j, v0.y + i, z_values[j - left.x]), color);
            }
        }
    }

    void Rasterizer::_ResizeZBuffer(uint32_t width, uint32_t height) const noexcept {
        m_z_buffer.resize(width * height, std::numeric_limits<float>::max());
    }    

    bool Rasterizer::BindWindow(win_framewrk::Window* window) noexcept {
        if (window == nullptr) {
            return false;
        }

        m_window_ptr = window;
        _ResizeZBuffer(window->GetWidth(), window->GetHeight());
        return true;
    }
    
    const win_framewrk::Window *Rasterizer::IsWindowBinded() const noexcept {
        return m_window_ptr;
    }

    void Rasterizer::SwapBuffers() const noexcept {
        m_window_ptr->PresentPixelBuffer();
        m_window_ptr->FillPixelBuffer(R_G_B_A(m_background_color));
    }

    void Rasterizer::ClearBackBuffer() const noexcept {
        std::fill(m_z_buffer.begin(), m_z_buffer.end(), std::numeric_limits<float>::max());
    }

    void Rasterizer::SetBackgroundColor(const math::color& color) noexcept {
        m_background_color = color;
    }
    
    const math::color& Rasterizer::GetBackgroundColor() const noexcept {
        return m_background_color;
    }
}
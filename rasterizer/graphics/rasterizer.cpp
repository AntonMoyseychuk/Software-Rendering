#include "rasterizer.hpp"

#include "math_3d/vec4.hpp"
#include "math_3d/vec_operations.hpp"
#include "math_3d/mat4.hpp"
#include "math_3d/mat_operations.hpp"

#include "math_3d/math.hpp"

#include <cassert>

namespace rasterization::gfx {
    Rasterizer::Rasterizer(win_framewrk::Window *window)
        : m_window_ptr(window)
    {
        BindWindow(window);
    }

    void Rasterizer::Render(RenderMode mode, size_t vbo_id, size_t ibo_id, const math::color& color) const noexcept {
        using namespace math;
        
        const auto& local_coords = m_vbos.at(vbo_id);
        const auto& indexes = m_ibos.at(ibo_id);

        const size_t vertex_count = local_coords.size();

        static std::vector<vec3f> transform_coords;
        static std::vector<vec3f> screen_coords;

    #pragma region resizing-buffers
        screen_coords.resize(vertex_count);
        transform_coords.resize(vertex_count);
        _ResizeZBuffer(m_window_ptr->GetWidth(), m_window_ptr->GetHeight());
    #pragma endregion resizing-buffers

        // Vertex Shader
        for (size_t i = 0; i < vertex_count; ++i) {
            _VertexShader(local_coords[i], transform_coords[i]);
        }

        // Rasterization
        _Rasterize(transform_coords, screen_coords);

        // Pixel Shader
        m_window_ptr->FillPixelBuffer(R_G_B_A(m_background_color));

        const static vec3f light_dir = normalize(vec3f::BACKWARD + vec3f::LEFT);

        switch (mode) {
        case RenderMode::POINTS:
            for (size_t i = 0; i < indexes.size(); ++i) {
                _PointPixelShader(screen_coords[indexes[i][0]], color);
            }    
            break;

        case RenderMode::LINES:
            for (size_t i = 0; i < indexes.size(); i += 2) {
                _LinePixelShader(screen_coords[indexes[i][0]], screen_coords[indexes[i + 1][0]], color);
            }
            break;

        case RenderMode::TRIANGLES:
            for (size_t i = 0; i < indexes.size(); i += 3) {
                const vec3f normal = normalize(cross(
                    local_coords[indexes[i + 2][0]] - local_coords[indexes[i][0]], 
                    local_coords[indexes[i + 1][0]] - local_coords[indexes[i][0]]
                ));
                const float light_intensity = dot(normal, light_dir) + 0.1f;
                
                _TrianglePixelShader(
                    screen_coords[indexes[i][0]], 
                    screen_coords[indexes[i + 1][0]], 
                    screen_coords[indexes[i + 2][0]],
                    color * light_intensity
                );
            }
            break;
        }
    }

    void Rasterizer::_VertexShader(const math::vec3f& local_coord, math::vec3f& transformed_coord) const noexcept {
        using namespace math;

        static const mat4f mvp = scale(mat4f::IDENTITY, vec4f(0.5f));
        transformed_coord = local_coord * mvp;
    }

    void Rasterizer::_Rasterize(const std::vector<math::vec3f> &transformed_coords, std::vector<math::vec3f> &screen_coords) const noexcept {
        const float dx = m_window_ptr->GetWidth() / 2.0f;
        const float dy = m_window_ptr->GetHeight() / 2.0f;

        for (size_t i = 0; i < transformed_coords.size(); ++i) {
            screen_coords[i] = math::vec3f(
                std::floorf((1.0f + transformed_coords[i].x) * dx), 
                std::floorf((1.0f - transformed_coords[i].y) * dy), 
                transformed_coords[i].z
            );
        }
    }

    void Rasterizer::_PointPixelShader(const math::vec3f& screen_coord, const math::color& color) const noexcept {
        const int64_t idx = screen_coord.x + screen_coord.y * m_window_ptr->GetWidth();
        if (idx >= 0 && idx < m_z_buffer.size()) {
            if (screen_coord.z >= m_z_buffer[static_cast<size_t>(idx)]) {
                m_z_buffer[static_cast<size_t>(idx)] = screen_coord.z;
                m_window_ptr->SetPixelColor(screen_coord.x, screen_coord.y, R_G_B_A(color));
            }
        }
    }

    void Rasterizer::_LinePixelShader(const math::vec3f& screen_coord_v0, const math::vec3f& screen_coord_v1, const math::color& color) const noexcept {
        using namespace math;

        auto v0 = screen_coord_v0;
        auto v1 = screen_coord_v1;

        static std::vector<int32_t> values;
        static std::vector<float> z_values;

        if (abs(v1.x - v0.x) > abs(v1.y - v0.y)) {
            if (v0.x > v1.x) {
                std::swap(v0, v1);
            }

            interpolate<int32_t>(v0.x, v0.y, v1.x, v1.y, values);
            interpolate<float>(v0.x, v0.z, v1.x, v1.z, z_values);
            for (int32_t x = v0.x; x <= v1.x; ++x) {
                _PointPixelShader(vec3f(x, values[x - v0.x], z_values[x - v0.x]), color);
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            interpolate<int32_t>(v0.y, v0.x, v1.y, v1.x, values);
            interpolate<float>(v0.y, v0.z, v1.y, v1.z, z_values);
            for (int32_t y = v0.y; y <= v1.y; ++y) {
                _PointPixelShader(vec3f(values[y - v0.y], y, z_values[y - v0.y]), color);
            }
        }
    }

    void Rasterizer::_TrianglePixelShader( 
        const math::vec3f &screen_coords0, 
        const math::vec3f &screen_coords1, 
        const math::vec3f &screen_coords2, 
        const math::color& triangle_color
    ) const noexcept {
        using namespace math;

        auto v0 = screen_coords0, v1 = screen_coords1, v2 = screen_coords2;
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
                _PointPixelShader(vec3f(j, v0.y + i, z_values[j - left.x]), triangle_color);
            }
        } 
    }

    void Rasterizer::_ResizeZBuffer(uint32_t width, uint32_t height) const noexcept {
        m_z_buffer.resize(width * height, -std::numeric_limits<float>::max());
    }

    size_t Rasterizer::CreateBuffer(BufferType type, const void* buffer, size_t size) noexcept {
        if (type == BufferType::VERTEX) {
            return _CreateVertexBuffer((const math::vec3f*)buffer, size / sizeof(math::vec3f));
        } else if (type == BufferType::INDEX) {
            return _CreateIndexBuffer((const math::vec3f*)buffer, size / sizeof(math::vec3f));
        } else {
            assert(false && "Invalid Buffer Type");
        }

        return 0;
    }

    size_t Rasterizer::_CreateVertexBuffer(const math::vec3f *buffer, size_t count) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX) + 1;
        } while (m_vbos.count(id) != 0);

        m_vbos[id] = std::vector<math::vec3f>(buffer, buffer + count);
    
        return id;
    }

    size_t Rasterizer::_CreateIndexBuffer(const math::vec3f *buffer, size_t count) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX) + 1;
        } while (m_ibos.count(id) != 0);

        m_ibos[id] = std::vector<math::vec3f>(buffer, buffer + count);

        return id;
    }

    std::vector<math::vec3f>& Rasterizer::GetVertexBuffer(size_t id) noexcept {
        return m_vbos.at(id);
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
    }

    void Rasterizer::ClearBackBuffer() const noexcept {
        std::fill(m_z_buffer.begin(), m_z_buffer.end(), -std::numeric_limits<float>::max());
    }

    void Rasterizer::SetBackgroundColor(const math::color& color) noexcept {
        m_background_color = color;
    }
    
    const math::color& Rasterizer::GetBackgroundColor() const noexcept {
        return m_background_color;
    }
}
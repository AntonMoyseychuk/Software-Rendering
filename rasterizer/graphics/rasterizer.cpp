#include "rasterizer.hpp"

#include <iostream>

namespace rasterization::gfx {
    Rasterizer::Rasterizer(win_framewrk::Window *window)
        : m_window_ptr(window)
    {
        BindWindow(window);
    }

    void Rasterizer::Render(RenderMode mode, size_t vbo_id, size_t ibo_id, math::Color color) const noexcept {
        const auto& verts = m_vbos.at(vbo_id);
        const auto& indexes = m_ibos.at(ibo_id);

        static std::vector<math::vec3i> screen_coords;
        static std::vector<math::vec3f> transform_coords;

        // Vertex Shader
        _VertexShader(transform_coords, vbo_id);

        // Rasterization
        _Rasterize(transform_coords, screen_coords);

        // Pixel Shader
        m_window_ptr->FillPixelBuffer(m_background_color.rgba);

        switch (mode) {
        case RenderMode::POINTS:
            for (size_t i = 0; i < indexes.size(); ++i) {
                _PointPixelShader(screen_coords[indexes[i]], color);
            }    
            break;

        case RenderMode::LINES:
            for (size_t i = 0; i < indexes.size(); i += 2) {
                // _LinePixelShader(screen_coords[indexes[i]], screen_coords[indexes[i + 1]], math::Color(math::Random(0, 255), math::Random(0, 255), math::Random(0, 255)));
                _RenderLine(screen_coords[indexes[i]], screen_coords[indexes[i + 1]], color);
            }
            break;

        case RenderMode::TRIANGLES:
            for (size_t i = 0; i < indexes.size(); i += 3) {
                // const math::vec3f normal(math::Cross(
                //     math::Normalize(verts[indexes[i + 2]] - verts[indexes[i]]),
                //     math::Normalize(verts[indexes[i + 1]] - verts[indexes[i]]))
                // );

                // const float intensity = math::Dot(normal, math::Normalize(math::VECTOR_LEFT + math::VECTOR_BACKWARD));

                // if (intensity > 0.0f) {
                //     _RenderTriangle(
                        // screen_coords[indexes[i]], 
                        // screen_coords[indexes[i + 1]], 
                        // screen_coords[indexes[i + 2]], 
                //         color * (intensity + 0.1f)
                //     );
                // }

                _TrianglePixelShader(
                    verts[indexes[i]], 
                    verts[indexes[i + 1]], 
                    verts[indexes[i + 2]], 
                    screen_coords[indexes[i]], 
                    screen_coords[indexes[i + 1]], 
                    screen_coords[indexes[i + 2]],
                    color,
                    math::Normalize(math::VECTOR_BACKWARD + math::VECTOR_LEFT)
                );
            }
            break;
        }
    }

    void Rasterizer::_VertexShader(std::vector<math::vec3f> &transformed_coords, size_t vbo_id) const noexcept {
        using namespace math;

        static const auto mvp = math::Scale(math::Identity<math::mat4f>(), math::vec3f(0.75f));
        
        const auto& vbo = m_vbos.at(vbo_id);
        transformed_coords.resize(vbo.size());

        for (size_t i = 0; i < vbo.size(); ++i) {
            transformed_coords[i] = vbo[i] * mvp;
        }
    }

    void Rasterizer::_Rasterize(const std::vector<math::vec3f> &transformed_coords, std::vector<math::vec3i> &screen_coords) const noexcept {
        screen_coords.resize(transformed_coords.size());

        const float dx = m_window_ptr->GetWidth() / 2.0f;
        const float dy = m_window_ptr->GetHeight() / 2.0f;

        for (size_t i = 0; i < transformed_coords.size(); ++i) {
            screen_coords[i] = math::vec3i(
                (1.0f + transformed_coords[i].x) * dx, 
                (1.0f - transformed_coords[i].y) * dy, 
                transformed_coords[i].z * 100.0f
            );
        }
    }

    void Rasterizer::_PointPixelShader(const math::vec3i& screen_coord, math::Color color) const noexcept {
        const int64_t idx = screen_coord.x + screen_coord.y * m_window_ptr->GetWidth();
        if (idx >= 0 && idx < m_z_buffer.size()) {
            if (screen_coord.z >= m_z_buffer[idx]) {
                m_z_buffer[idx] = screen_coord.z;
                m_window_ptr->SetPixelColor(screen_coord.x, screen_coord.y, color.rgba);
            }
        }
    }

    void Rasterizer::_LinePixelShader(const math::vec3i& screen_coord_v0, const math::vec3i& screen_coord_v1, math::Color color) const noexcept {
        auto v0 = screen_coord_v0;
        auto v1 = screen_coord_v1;

        static std::vector<int32_t> values, values2;

        if (math::Abs(v1.x - v0.x) > math::Abs(v1.y - v0.y)) {
            if (v0.x > v1.x) {
                std::swap(v0, v1);
            }

            math::Interpolate<int32_t>(v0.x, v0.y, v1.x, v1.y, values);
            math::Interpolate<int32_t>(v0.x, v0.z, v1.x, v1.z, values2);
            for (int32_t x = v0.x; x <= v1.x; ++x) {
                // m_window_ptr->SetPixelColor(x, values[x - v0.x], color.r, color.g, color.b, color.a);
                _PointPixelShader(math::vec3i(x, values[x - v0.x], values2[x - v0.x]), color);
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            math::Interpolate<int32_t>(v0.y, v0.x, v1.y, v1.x, values);
            math::Interpolate<int32_t>(v0.y, v0.z, v1.y, v1.z, values2);
            for (int32_t y = v0.y; y <= v1.y; ++y) {
                // m_window_ptr->SetPixelColor(values[y - v0.y], y, color.r, color.g, color.b, color.a);
                _PointPixelShader(math::vec3i(values[y - v0.y], y, values2[y - v0.y]), color);
            }
        }
    }

    void Rasterizer::_TrianglePixelShader(
        const math::vec3f &local_coord0, const math::vec3f &local_coord1, const math::vec3f &local_coord2, 
        const math::vec3i &screen_coords0, const math::vec3i &screen_coords1, const math::vec3i &screen_coords2, 
        math::Color color, 
        const math::vec3f &light_direction
    ) const noexcept {
        // const math::vec3f normal(math::Cross(
        //     math::Normalize(local_coord2 - local_coord0),
        //     math::Normalize(local_coord1 - local_coord0))
        // );

        // const float intensity = math::Dot(normal, light_direction);

        // auto v0 = screen_coords0, v1 = screen_coords1, v2 = screen_coords2;
        // if (v0.y > v1.y) { std::swap(v0, v1); }
        // if (v0.y > v2.y) { std::swap(v0, v2); }
        // if (v1.y > v2.y) { std::swap(v1, v2); }

        // const int32_t total_height = v2.y - v0.y;
        // for (int32_t i = 0; i < total_height; ++i) {
        //     bool is_second_half = i > v1.y - v0.y || v1.y == v0.y;
        //     const int32_t segment_height = is_second_half ? v2.y - v1.y : v1.y - v0.y;
            
        //     const float alpha = static_cast<float>(i) / total_height;
        //     const float beta  = static_cast<float>(i - (is_second_half ? v1.y - v0.y : 0)) / segment_height;
            
        //     math::vec3i left = v0 + (v2 - v0) * alpha;
        //     math::vec3i right = is_second_half ? v1 + (v2 - v1) * beta : v0 + (v1 - v0) * beta;
        //     if (left.x > right.x) { std::swap(left, right); }
            
        //     for (int32_t j = left.x; j <= right.x; ++j) {
        //         float phi = B.x == A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
        //         Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
        //         _PointPixelShader(math::vec3i(j, v0.y + i, ), color * intensity);
        //         // _RenderPoint(math::vec2i(j, v0.y + i), color);
        //     }
        // } 
    }

    void Rasterizer::_RenderLine(const math::vec3i &_v0, const math::vec3i &_v1, math::Color color) const noexcept {
        auto v0 = _v0;
        auto v1 = _v1;

        const float dx = static_cast<float>(v1.x - v0.x);
        const float dy = static_cast<float>(v1.y - v0.y);

        const int32_t width = m_window_ptr->GetWidth();
        const int32_t height = m_window_ptr->GetHeight();

        static std::vector<int32_t> values;

        if (math::Abs(dx) > math::Abs(dy)) {
            if (v0.x > v1.x) {
                std::swap(v0, v1);
            }

            math::Interpolate<int32_t>(v0.x, v0.y, v1.x, v1.y, values);
            for (int32_t x = v0.x; x <= v1.x; ++x) {
                m_window_ptr->SetPixelColor(x, values[x - v0.x], color.r, color.g, color.b, color.a);
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            math::Interpolate<int32_t>(v0.y, v0.x, v1.y, v1.x, values);
            for (int32_t y = v0.y; y <= v1.y; ++y) {
                m_window_ptr->SetPixelColor(values[y - v0.y], y, color.r, color.g, color.b, color.a);
            }
        }
    }

    // void Rasterizer::_RenderTriangle(const math::vec3i &_v0, const math::vec3i &_v1, const math::vec3i &_v2, math::Color color) const noexcept {
    //     auto v0 = _v0, v1 = _v1, v2 = _v2;
    //     if (v0.y > v1.y) { std::swap(v0, v1); }
    //     if (v0.y > v2.y) { std::swap(v0, v2); }
    //     if (v1.y > v2.y) { std::swap(v1, v2); }

    //     const int32_t total_height = v2.y - v0.y;
    //     for (int32_t i = 0; i < total_height; ++i) {
    //         bool is_second_half = i > v1.y - v0.y || v1.y == v0.y;
    //         const int32_t segment_height = is_second_half ? v2.y - v1.y : v1.y - v0.y;
            
    //         const float alpha = static_cast<float>(i) / total_height;
    //         const float beta  = static_cast<float>(i - (is_second_half ? v1.y - v0.y : 0)) / segment_height;
            
    //         math::vec3i left = v0 + (v2 - v0) * alpha;
    //         math::vec3i right = is_second_half ? v1 + (v2 - v1) * beta : v0 + (v1 - v0) * beta;
    //         if (left.x > right.x) { std::swap(left, right); }
            
    //         for (int32_t j = left.x; j <= right.x; ++j) {
    //             _RenderPoint(math::vec2i(j, v0.y + i), color);
    //         }
    //     } 
    // }

    size_t Rasterizer::CreateBuffer(BufferType type, const void* buffer, size_t size) noexcept {
        if (type == BufferType::VERTEX) {
            return _CreateVertexBuffer(buffer, size);
        } else if (type == BufferType::INDEX) {
            return _CreateIndexBuffer(buffer, size / sizeof(size_t));
        } else {
            assert(false && "Invalid Buffer Type");
        }

        return 0;
    }

    size_t Rasterizer::_CreateVertexBuffer(const void *buffer, size_t size) noexcept {
        size_t id;
        do {
            id = math::Random((size_t)0, SIZE_MAX) + 1;
        } while (m_vbos.count(id) != 0);

        m_vbos[id] = std::vector<math::vec3f>((math::vec3f*)buffer, (math::vec3f*)buffer + size / sizeof(math::vec3f));
    
        return id;
    }

    size_t Rasterizer::_CreateIndexBuffer(const void *buffer, size_t count) noexcept {
        size_t id;
        do {
            id = math::Random((size_t)0, SIZE_MAX) + 1;
        } while (m_ibos.count(id) != 0);

        m_ibos[id] = std::vector<size_t>((size_t*)buffer, (size_t*)buffer + count);

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

        m_z_buffer.resize(window->GetWidth() * window->GetHeight(), INT32_MIN);
        return true;
    }
    
    const win_framewrk::Window *Rasterizer::IsWindowBinded() const noexcept {
        return m_window_ptr;
    }

    void Rasterizer::SwapBuffers() const noexcept {
        m_window_ptr->PresentPixelBuffer();
    }

    void Rasterizer::ClearBackBuffer() const noexcept {
        std::fill(m_z_buffer.begin(), m_z_buffer.end(), INT32_MIN);
    }

    void Rasterizer::SetBackgroundColor(math::Color color) noexcept {
        m_background_color = color;
    }
    
    math::Color Rasterizer::GetBackgroundColor() const noexcept {
        return m_background_color;
    }
}
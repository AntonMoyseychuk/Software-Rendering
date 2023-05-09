#include "rasterizer.hpp"

#include <iostream>

namespace rasterization::gfx {
    Rasterizer::Rasterizer(win_framewrk::Window *window)
        : m_window_ptr(window)
    {
        BindWindow(window);
    }
    
    void Rasterizer::BindWindow(win_framewrk::Window* window) noexcept {
        m_window_ptr = window;
    }
    
    const win_framewrk::Window *Rasterizer::IsWindowBinded() const noexcept {
        return m_window_ptr;
    }

    size_t Rasterizer::AddVertexBuffer(const std::vector<math::vec3f> &vbo) noexcept {
        size_t id;
        do {
            id = math::Random((size_t)0, SIZE_MAX);
        } while (m_vbos.count(id) != 0);
        
        m_vbos[id] = vbo;
        return id;
    }

    size_t Rasterizer::AddIndexBuffer(const std::vector<size_t> &ibo) noexcept {
        size_t id;
        do {
            id = math::Random((size_t)0, SIZE_MAX);
        } while (m_vbos.count(id) != 0);
        
        m_ibos[id] = ibo;
        return id;
    }

    std::vector<math::vec3f>* Rasterizer::GetVertexBuffer(size_t id) noexcept {
        return m_vbos.count(id) == 0 ? nullptr : &m_vbos[id];        
    }

    void Rasterizer::_RenderPoint(const math::vec2i &_point, math::Color _color) const noexcept {
        m_window_ptr->SetPixelColor(_point.x, _point.y, _color.rgba);
    }

    void Rasterizer::_Interpolate(int32_t i0, int32_t d0, int32_t i1, int32_t d1, std::vector<int32_t> &values) noexcept
    {
        if (i0 == i1) {
            values.resize(1);
            values[0] = d0;
            return;
        }

        values.resize(i1 - i0 + 1);
        
        const float a = static_cast<float>(d1 - d0) / (i1 - i0);
        float d = d0;
        for (int32_t i = 0; i <= i1 - i0; ++i, d += a) {
            values[i] = d;
        }
    }

    void Rasterizer::_RenderLine(const math::vec2i &_v0, const math::vec2i &_v1, math::Color color) const noexcept {
        auto v0 = _v0;
        auto v1 = _v1;

        const float dx = static_cast<float>(v1.x - v0.x);
        const float dy = static_cast<float>(v1.y - v0.y);

        static std::vector<int32_t> values;

        const int32_t width = m_window_ptr->GetWidth();
        const int32_t height = m_window_ptr->GetHeight();

        if (math::Abs(dx) > math::Abs(dy)) {
            if (v0.x > v1.x) {
                std::swap(v0, v1);
            }

            _Interpolate(v0.x, v0.y, v1.x, v1.y, values);
            for (int32_t x = v0.x; x <= v1.x; ++x) {
                m_window_ptr->SetPixelColor(x, values[x - v0.x], color.r, color.g, color.b, color.a);
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            _Interpolate(v0.y, v0.x, v1.y, v1.x, values);
            for (int32_t y = v0.y; y <= v1.y; ++y) {
                m_window_ptr->SetPixelColor(values[y - v0.y], y, color.r, color.g, color.b, color.a);
            }
        }
    }

    void Rasterizer::_RenderTriangle(
        const math::vec2i &_v0, 
        const math::vec2i &_v1, 
        const math::vec2i &_v2, 
        math::Color color
    ) const noexcept {
        auto v0 = _v0, v1 = _v1, v2 = _v2;
        if (v0.y > v1.y) { std::swap(v0, v1); }
        if (v0.y > v2.y) { std::swap(v0, v2); }
        if (v1.y > v2.y) { std::swap(v1, v2); }

        int32_t total_height = v2.y - v0.y;
        for (size_t i = 0; i < total_height; ++i) {
            bool second_half = i > v1.y - v0.y || v1.y == v0.y;
            int segment_height = second_half ? v2.y - v1.y : v1.y - v0.y;
            float alpha = (float)i / total_height;
            float beta  = (float)(i - (second_half ? v1.y - v0.y : 0)) / segment_height;
            math::vec2i A = v0 + (v2 - v0) * alpha;
            math::vec2i B = second_half ? v1 + (v2 - v1) * beta : v0 + (v1 - v0) * beta;
            if (A.x > B.x) std::swap(A, B);
            for (int32_t j = A.x; j <= B.x; ++j) {
                _RenderPoint(math::vec2i(j, v0.y + i), color);
            }
        }
    }

    void Rasterizer::Render(RenderMode mode, size_t vbo_id, size_t ibo_id, math::Color color) const noexcept {
        assert(m_vbos.count(vbo_id) == 1);
        assert(m_ibos.count(ibo_id) == 1);
        
        const auto& verts = m_vbos.at(vbo_id);
        const auto& indexes = m_ibos.at(ibo_id);

        static win_framewrk::Window* window = win_framewrk::Window::Get();
        const float dx = window->GetWidth() / 2.0f;
        const float dy = window->GetHeight() / 2.0f;

        switch (mode) {
        case RenderMode::POINTS:
            for (size_t i = 0; i < indexes.size(); ++i) {
                math::vec2i point((1.0f + verts[indexes[i]].x) * dx, (1.0f - verts[indexes[i]].y) * dy);
                _RenderPoint(point, color);
            }    
            break;

        case RenderMode::LINES:
            for (size_t i = 0; i < indexes.size(); i += 2) {
                math::vec2i line[2];
                for (size_t j = 0; j < 2; ++j) {
                    line[j].x = (1.0f + verts[indexes[i + j]].x) * dx;
                    line[j].y = (1.0f - verts[indexes[i + j]].y) * dy;
                }

                _RenderLine(line[0], line[1], color);
            }
            break;

        case RenderMode::TRIANGLES:
            for (size_t i = 0; i < indexes.size(); i += 3) {
                math::vec2i polygon[3];
                for (size_t j = 0; j < 3; ++j) {
                    polygon[j].x = (1.0f + verts[indexes[i + j]].x) * dx;
                    polygon[j].y = (1.0f - verts[indexes[i + j]].y) * dy;
                }

                const math::vec3f normal(math::Cross(
                    math::Normalize(verts[indexes[i + 2]] - verts[indexes[i]]),
                    math::Normalize(verts[indexes[i + 1]] - verts[indexes[i]]))
                );

                const float intensity = math::Dot(normal, math::Normalize(math::VECTOR_LEFT + math::VECTOR_BACKWARD));

                if (intensity > 0.0f) {
                    _RenderTriangle(polygon[0], polygon[1], polygon[2], color * (intensity + 0.1f));
                }
            }
            break;
        }
    }
}
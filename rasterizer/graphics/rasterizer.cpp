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

    std::vector<math::vec3f>& Rasterizer::GetVertexBuffer(size_t id) noexcept {
        return m_vbos.at(id);
    }

    void Rasterizer::_RenderPoint(const math::vec2i &_point, math::Color _color) const noexcept {
        m_window_ptr->SetPixelColor(_point.x, _point.y, _color.rgba);
    }

    std::vector<int32_t> &Rasterizer::_Interpolate(int32_t _i0, int32_t _d0, int32_t _i1, int32_t _d1, std::vector<int32_t> &_values) noexcept {
        if (_i0 == _i1) {
            _values.resize(1);
            _values[0] = _d0;
            return _values;
        }

        _values.resize(_i1 - _i0 + 1);
        
        const float a = static_cast<float>(_d1 - _d0) / (_i1 - _i0);
        for (int32_t i = 0; i <= _i1 - _i0; ++i) {
            _values[i] = static_cast<int32_t>(_d0);
            _d0 += a;
        }

        return _values;
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
        const math::vec3i &_v0, 
        const math::vec3i &_v1, 
        const math::vec3i &_v2, 
        math::Color color
    ) const noexcept {
        auto v0 = _v0, v1 = _v1, v2 = _v2;
        if (v0.y > v1.y) { std::swap(v0, v1); }
        if (v0.y > v2.y) { std::swap(v0, v2); }
        if (v1.y > v2.y) { std::swap(v1, v2); }

        int32_t total_height = v2.y - v0.y;
        for (int32_t i = 0; i < total_height; ++i) {
            bool second_half = i > v1.y - v0.y || v1.y == v0.y;
            int segment_height = second_half ? v2.y - v1.y : v1.y - v0.y;
            
            float alpha = (float)i / total_height;
            float beta  = (float)(i - (second_half ? v1.y - v0.y : 0)) / segment_height;
            
            math::vec3i A = v0 + (v2 - v0) * alpha;
            math::vec3i B = second_half ? v1 + (v2 - v1) * beta : v0 + (v1 - v0) * beta;
            if (A.x > B.x) { std::swap(A, B); } 
            
            for (int32_t j = A.x; j <= B.x; ++j) {
                _RenderPoint(math::vec2i(j, v0.y + i), color);
            }
        }
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

    void Rasterizer::Render(RenderMode mode, size_t vbo_id, size_t ibo_id, math::Color color) const noexcept {
        const float dx = m_window_ptr->GetWidth() / 2.0f;
        const float dy = m_window_ptr->GetHeight() / 2.0f;
        
        auto verts = m_vbos.at(vbo_id);
        auto indexes = m_ibos.at(ibo_id);

        // Vertex Shader
        for (size_t i = 0; i < verts.size(); ++i) {
            verts[i] = verts[i] * math::Scale(math::Identity<math::mat4f>(), math::vec3f(0.75f));
        }

        // Pixel Shader
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
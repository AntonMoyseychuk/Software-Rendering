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

        // Vertex Shader
        static const auto mvp = std::make_shared<math::mat4f>(math::Scale(math::Identity<math::mat4f>(), math::vec3f(0.75f)));

        static std::vector<math::vec3i> screen_coords;
        _VertexShader(screen_coords, vbo_id, mvp);

        // Pixel Shader
        m_window_ptr->FillPixelBuffer(m_background_color.rgba);

        switch (mode) {
        case RenderMode::POINTS:
            for (size_t i = 0; i < indexes.size(); ++i) {
                _RenderPoint(screen_coords[indexes[i]], color);
            }    
            break;

        case RenderMode::LINES:
            for (size_t i = 0; i < indexes.size(); i += 2) {
                _RenderLine(screen_coords[indexes[i]], screen_coords[indexes[i + 1]], color);
            }
            break;

        case RenderMode::TRIANGLES:
            for (size_t i = 0; i < indexes.size(); i += 3) {
                const math::vec3f normal(math::Cross(
                    math::Normalize(verts[indexes[i + 2]] - verts[indexes[i]]),
                    math::Normalize(verts[indexes[i + 1]] - verts[indexes[i]]))
                );

                const float intensity = math::Dot(normal, math::Normalize(math::VECTOR_LEFT + math::VECTOR_BACKWARD));

                if (intensity > 0.0f) {
                    _RenderTriangle(
                        screen_coords[indexes[i]], 
                        screen_coords[indexes[i + 1]], 
                        screen_coords[indexes[i + 2]], 
                        color * (intensity + 0.1f)
                    );
                }
            }
            break;
        }
    }

    void Rasterizer::_VertexShader(std::vector<math::vec3i> &out_coords, size_t vbo_id, std::shared_ptr<math::mat4f> mvp) const noexcept {
        using namespace math;
        
        const auto& vbo = m_vbos.at(vbo_id);
        
        if (vbo.size() != out_coords.size()) {
            out_coords.resize(vbo.size());
        }

        const float dx = m_window_ptr->GetWidth() / 2.0f;
        const float dy = m_window_ptr->GetHeight() / 2.0f;

        for (std::size_t i = 0; i < vbo.size(); ++i) {
            vec3f pos = vbo[i];
            if (mvp != nullptr) {
                pos = pos * (*mvp);
            }
            out_coords[i] = math::vec3i((1.0f + pos.x) * dx, (1.0f - pos.y) * dy, pos.z * 100);
        }
    }

    void Rasterizer::_RenderPoint(const math::vec3i &point, math::Color color) const noexcept {
        m_window_ptr->SetPixelColor(point.x, point.y, color.rgba);
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

    void Rasterizer::_RenderTriangle(const math::vec3i &_v0, const math::vec3i &_v1, const math::vec3i &_v2, math::Color color) const noexcept {
        auto v0 = _v0, v1 = _v1, v2 = _v2;
        if (v0.y > v1.y) { std::swap(v0, v1); }
        if (v0.y > v2.y) { std::swap(v0, v2); }
        if (v1.y > v2.y) { std::swap(v1, v2); }

        const int32_t total_height = v2.y - v0.y;
        for (int32_t i = 0; i < total_height; ++i) {
            bool is_second_half = i > v1.y - v0.y || v1.y == v0.y;
            const int32_t segment_height = is_second_half ? v2.y - v1.y : v1.y - v0.y;
            
            const float alpha = static_cast<float>(i) / total_height;
            const float beta  = static_cast<float>(i - (is_second_half ? v1.y - v0.y : 0)) / segment_height;
            
            math::vec3i left = v0 + (v2 - v0) * alpha;
            math::vec3i right = is_second_half ? v1 + (v2 - v1) * beta : v0 + (v1 - v0) * beta;
            if (left.x > right.x) { std::swap(left, right); } 
            
            for (int32_t j = left.x; j <= right.x; ++j) {
                _RenderPoint(math::vec2i(j, v0.y + i), color);
            }
        } 
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
        return true;
    }
    
    const win_framewrk::Window *Rasterizer::IsWindowBinded() const noexcept {
        return m_window_ptr;
    }

    void Rasterizer::SwapBuffers() const noexcept {
        m_window_ptr->PresentPixelBuffer();
    }
    
    void Rasterizer::SetBackgroundColor(math::Color color) noexcept {
        m_background_color = color;
    }
    
    math::Color Rasterizer::GetBackgroundColor() const noexcept {
        return m_background_color;
    }
}
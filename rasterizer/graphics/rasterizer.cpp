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

    void Rasterizer::_Interpolate(int32_t i0, int32_t d0, int32_t i1, int32_t d1, std::vector<int32_t> &values) noexcept {
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

    void Rasterizer::_RenderWireframeTriangle(
        const math::vec2i &_v0, 
        const math::vec2i &_v1, 
        const math::vec2i &_v2, 
        math::Color color
    ) const noexcept {
        _RenderLine(_v0, _v1, color);
        _RenderLine(_v1, _v2, color);
        _RenderLine(_v2, _v0, color);
    }

    void Rasterizer::Render(RenderMode mode, const Model &model, math::Color color) const noexcept {
        if (mode == RenderMode::LINES) {
            const auto& verts = model.GetVertexes();
            const auto& indexes = model.GetIndexes();

            assert(indexes.size() % 3 == 0);

            static win_framewrk::Window* window = win_framewrk::Window::Get();
            const float dx = window->GetWidth() / 2.0f;
            const float dy = window->GetHeight() / 2.0f;

            math::vec2i polygon[3];
            for (size_t i = 0; i < indexes.size(); i += 3) {
                for (size_t j = 0; j < 3; ++j) {
                    polygon[j].x = (1.0f + verts[indexes[i + j]].x) * dx;
                    polygon[j].y = (1.0f - verts[indexes[i + j]].y) * dy;
                }

                _RenderWireframeTriangle(polygon[0], polygon[1], polygon[2], color);
            }
        } else {
            assert(false && "Invalid render mode");
        }
    }
}
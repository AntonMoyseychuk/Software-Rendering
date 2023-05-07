#include "application.hpp"
#include "math_3d/math.hpp"

#include "graphics/model.hpp"

#include <iostream>
#include <memory>


namespace rasterization {
    struct Color {
        float r, g, b, a;
    };

    static void Interpolate(int32_t i0, int32_t d0, int32_t i1, int32_t d1, std::vector<int32_t>& values) noexcept {
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

    static void RenderLine(const math::vec2i& _v0, const math::vec2i& _v1, const Color& color) noexcept {
        static win_framewrk::Window* window = win_framewrk::Window::Get();

        auto v0 = _v0;
        auto v1 = _v1;

        const float dx = static_cast<float>(v1.x - v0.x);
        const float dy = static_cast<float>(v1.y - v0.y);

        static std::vector<int32_t> values;

        if (math::Abs(dx) > math::Abs(dy)) {
            if (v0.x > v1.x) {
                std::swap(v0, v1);
            }

            Interpolate(v0.x, v0.y, v1.x, v1.y, values);

            for (int32_t x = v0.x; x <= v1.x; ++x) {
                window->SetPixelColor(x, values[x - v0.x], color.r * 255, color.g * 255, color.b * 255, color.a * 255);
            }
        } else {
            if (v0.y > v1.y) {
                std::swap(v0, v1);
            }
            
            Interpolate(v0.y, v0.x, v1.y, v1.x, values);

            for (int32_t y = v0.y; y <= v1.y; ++y) {
                window->SetPixelColor(values[y - v0.y], y, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
            }
        }

    }

    static void RenderWireframeTriangle(const math::vec2i& _v0, const math::vec2i& _v1, const math::vec2i& _v2, const Color& color) noexcept {
        RenderLine(_v0, _v1, color);
        RenderLine(_v1, _v2, color);
        RenderLine(_v2, _v0, color);
    }

    static void RenderWireframeModel(const gfx::Model& model, const Color& color) noexcept {
        const auto verts = model.GetVertexes();
        const auto indexes = model.GetIndexes();

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

            RenderWireframeTriangle(polygon[0], polygon[1], polygon[2], color);
        }
    }


    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        m_window->Init(title, width, height);
    }

    void Application::Run() noexcept {
        using namespace math;

        gfx::Model model("..\\..\\..\\rasterizer\\assets\\human.obj");
        
        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            RenderWireframeModel(model, Color{ 1.0f, 1.0f, 1.0f, 1.0f });
            
            m_window->PresentPixelBuffer();          
        }
    }
    
    void Application::SetFPSLock(size_t fps) const noexcept {
        m_fps_lock = 1.0f / fps;
    }
    
    float Application::_LockFPS() const noexcept {
        using namespace std::chrono;

        float dt = 0.0f;
        steady_clock::time_point curr_frame;
        
        do {
            curr_frame = steady_clock::now();
            dt = duration<float>(curr_frame - m_last_frame).count();
        } while(dt < m_fps_lock);
        m_last_frame = curr_frame;

        return dt;
    }
}
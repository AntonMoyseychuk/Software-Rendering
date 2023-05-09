#include "application.hpp"
#include "math_3d/math.hpp"

#include "graphics/model.hpp"

#include <iostream>
#include <memory>


namespace rasterization {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        m_window->Init(title, width, height);
        m_rasterizer.BindWindow(m_window);

        assert(m_rasterizer.IsWindowBinded() != nullptr);
    }

    void Application::Run() noexcept {
        using namespace math;
        using namespace win_framewrk;

        gfx::Model model("..\\..\\..\\rasterizer\\assets\\human.obj");
        auto& model_verts = model.GetVertexes();

        for (size_t i = 0; i < model_verts.size(); ++i) {
            model_verts[i] = model_verts[i] * Scale(Identity<mat4f>(), vec3f(0.75f));
        }

        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            const float angle = ToRadians(dt) * 10.0f;

            if (m_window->IsKeyPressed(Key::D)) {
                for (size_t i = 0; i < model_verts.size(); ++i) {
                    model_verts[i] = model_verts[i] * Quaternion(cosf(angle), 0.0f, sinf(angle), 0.0f);
                }
            } else if (m_window->IsKeyPressed(Key::A)) {
                for (size_t i = 0; i < model_verts.size(); ++i) {
                    model_verts[i] = model_verts[i] * Quaternion(cosf(-angle), 0.0f, sinf(-angle), 0.0f);
                }
            }

            if (m_window->IsKeyPressed(Key::W)) {
                for (size_t i = 0; i < model_verts.size(); ++i) {
                    model_verts[i] = model_verts[i] * Quaternion(cosf(-angle), sinf(-angle), 0.0f, 0.0f);
                }
            } else if (m_window->IsKeyPressed(Key::S)) {
                for (size_t i = 0; i < model_verts.size(); ++i) {
                    model_verts[i] = model_verts[i] * Quaternion(cosf(angle), sinf(angle), 0.0f, 0.0f);
                }
            }

            m_rasterizer.Render(gfx::RenderMode::LINES, model, math::Color::WHITE);

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
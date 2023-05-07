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

        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            const static float angle = ToRadians(dt);

            if (m_window->IsKeyPressed(Key::D)) {
                static math::Quaternion q_right(cosf(angle), 0.0f, sinf(angle), 0.0f);
                for (auto& vert : model.GetVertexes()) {
                    vert = vert * q_right;
                }
            } else if (m_window->IsKeyPressed(Key::A)) {
                static math::Quaternion q_left(cosf(-angle), 0.0f, sinf(-angle), 0.0f);
                for (auto& vert : model.GetVertexes()) {
                    vert = vert * q_left;
                }
            }

            m_rasterizer.Render(gfx::RenderMode::LINES, model, math::Color::RED);

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
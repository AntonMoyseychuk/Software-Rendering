#include "application.hpp"
#include "math_3d/math.hpp"

#include <iostream>
#include <memory>


namespace rasterization {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        m_window->Init(title, width, height);
    }

    void Application::Run() noexcept {
        using namespace math;

        // vec2ui last_window_size(m_window->GetWidth(), m_window->GetHeight()), curr_window_size = last_window_size;

        vec2ui p1(10, 300), p2(500, 300);
        float a = static_cast<float>(p2.y - p1.y) / (p2.x - p1.x);
        float b = p1.y - a * p1.x;

        while (m_window->IsOpen()) {
            m_window->PollEvent();

            for (uint32_t x = p1.x; x < p2.x; ++x) {
                const uint32_t y = static_cast<uint32_t>(a * x + b); 
                m_window->SetPixelColor(x, y, 255, 0, 0, 255);
            }
            
            m_window->PresentPixelBuffer();          
            
            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;
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
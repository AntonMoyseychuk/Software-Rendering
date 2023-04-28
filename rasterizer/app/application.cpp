#include "application.hpp"

#include "math_3d/math.hpp"

#include <iostream>
#include <memory>


namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), 
            // m_renderer(), 
            // m_scene(), 
            // m_camera(math::vec3f(0.0f), math::vec3f(0.0f, 0.0f, -5.0f), math::VECTOR_UP, 90.0f, (float)width / height),
            m_last_frame(std::chrono::steady_clock::now())
    {
        using namespace math;

        m_window->Init(title, width, height);
    }
    
    void Application::Run() noexcept {
        math::vec2ui last_window_size;

        while (m_window->IsOpen()) {
            const auto curr_frame = std::chrono::steady_clock::now();
            const auto dt = std::chrono::duration<float>(curr_frame - m_last_frame).count();
            m_last_frame = curr_frame;

            m_window->PollEvent();

            // m_window->FillPixelBuffer(frame);
            m_window->PresentPixelBuffer();          
            
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;
        }
    }   
}
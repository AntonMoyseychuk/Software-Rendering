#include "application.hpp"
#include "graphics/color.hpp"

#include "math_3d/math.hpp"

#include <iostream>
#include <memory>


namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now())
    {
        m_window->Init(title, width, height);
    }

    void Application::Run() noexcept {
        using namespace gfx;
        using namespace math;

        vec2ui last_window_size(m_window->GetWidth(), m_window->GetHeight()), curr_window_size = last_window_size;

        while (m_window->IsOpen()) {
            const auto curr_frame = std::chrono::steady_clock::now();
            const auto dt = std::chrono::duration<float>(curr_frame - m_last_frame).count();
            m_last_frame = curr_frame;

            m_window->PollEvent();

            for (size_t i = 0; i< m_window->GetHeight(); ++i) {
                m_window->SetPixelColor(i, i, 255, 0, 0, 255);
            }
            
            m_window->PresentPixelBuffer();          
            
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;
        }
    }   
}
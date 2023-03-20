#include "application.hpp"
#include "objects/sphere.hpp"
#include "objects/point_light.hpp"

#include <iostream>
#include <memory>

namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_renderer(), m_scene(), 
            m_camera(math::VECTOR_FORWARD * 2.5f, math::vec3f(0.0f), math::VECTOR_UP, 45.0f, (float)width / height), 
                m_last_frame(std::chrono::steady_clock::now())
    {
        m_window->Init(title, width, height);
        m_window->SetBackgroundColor(gfx::Color(80).rgba);
        
        m_renderer.SetAntialiasingLevel(gfx::AntialiasingLevel::X2);

        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.0f, -1.0f), 0.5f, gfx::Material(gfx::Color::MAGENTA, 1.5f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-1.0f, 0.3f, -2.0f), 0.5f, gfx::Material(gfx::Color::GREEN, 1.5f)));

        m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(5.0f, 5.0f, 5.0f), gfx::Color::WHITE, 1.0f));
    }
    
    void Application::Run() noexcept {
        auto background = gfx::LoadColorFromUInt32(m_window->GetBackgroundColor());

        while (m_window->IsOpen()) {
            m_window->PollEvent();

            math::vec2ui actual_window_size(m_window->GetWidth(), m_window->GetHeight());
            m_camera.SetAspectRatio((float)actual_window_size.x / actual_window_size.y);

            m_renderer.SetOutputFrameSize(actual_window_size);

            auto& buffer = m_renderer.Render(m_scene, m_camera, background);
            
            m_window->FillPixelBuffer(buffer);
            m_window->PresentPixelBuffer();
            
            m_scene.GetLights().begin()->get()->MoveFor(math::VECTOR_LEFT * 0.1f);

            auto curr_frame = std::chrono::steady_clock::now();
            std::cout << "FPS: " << std::to_string(1.0f / std::chrono::duration<float>(curr_frame - m_last_frame).count()) << std::endl;
            m_last_frame = curr_frame;
        }
    }
}
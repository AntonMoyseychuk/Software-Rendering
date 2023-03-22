#include "application.hpp"
#include "objects/sphere.hpp"
#include "objects/point_light.hpp"
#include "objects/directional_light.hpp"
#include "objects/ambient_light.hpp"

#include <iostream>
#include <memory>

namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_renderer(), m_scene(), 
            m_camera(math::VECTOR_FORWARD * 20.0f, math::vec3f(0.0f), math::VECTOR_UP, 45.0f, (float)width / height), 
                m_last_frame(std::chrono::steady_clock::now())
    {
        m_window->Init(title, width, height);
        m_window->SetBackgroundColor(gfx::Color(80).rgba);
        
        m_renderer.SetAntialiasingLevel(gfx::AntialiasingLevel::X4);

        for (std::size_t i = 0; i < 100; ++i) {
            m_scene.AddDrawble(std::make_shared<gfx::Sphere>(
                    math::vec3f(math::Random(-9.0f, 9.0f), math::Random(-9.0f, 9.0f), math::Random(-9.0f, 9.0f)),
                    math::Random(0.5f, 1.0f), 
                    gfx::Material(gfx::Color(math::Random(0, 255), math::Random(0, 255), math::Random(0, 255)), 500.0f)
                )
            );
        }
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, -0.5f, -1.0f), 0.5f, gfx::Material(gfx::Color::MAGENTA, 100)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-1.0f, 0.3f, -2.5f), 0.5f, gfx::Material(gfx::Color::GREEN, 100)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-2.0f, 2.3f, -5.0f), 0.5f, gfx::Material(gfx::Color::RED, 100)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(2.0f, 1.3f, -2.0f), 0.5f, gfx::Material(gfx::Color::YELLOW, 100)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 3.0f, -7.0f), 0.5f, gfx::Material(gfx::Color::CYAN, 100)));


        m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(math::vec3f(1.0f, 0.0f, -2.0f), gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(math::VECTOR_RIGHT, gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(8.0f, -10.0f, 8.0f), gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(-3.0f, 1.0f, 0.0f), gfx::Color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<gfx::AmbientLight>(gfx::Color::WHITE, 0.1f));
    }
    
    void Application::Run() noexcept {
        auto background = gfx::UInt32ToColor(m_window->GetBackgroundColor());
        math::vec2ui actual_window_size;

        while (m_window->IsOpen()) {
            m_window->PollEvent();

            actual_window_size.x = m_window->GetWidth();
            actual_window_size.y = m_window->GetHeight();
            m_camera.SetAspectRatio((float)actual_window_size.x / actual_window_size.y);

            m_renderer.SetOutputFrameSize(actual_window_size);

            auto& buffer = m_renderer.Render(m_scene, m_camera, background);
            
            m_window->FillPixelBuffer(buffer);
            m_window->PresentPixelBuffer();
            
            // {
            //     dynamic_cast<gfx::DirectionalLigth*>(m_scene.GetLights().begin()->get())->SetDirection(math::vec3f(
            //             math::Random(-1.0f, 1.0f), math::Random(-1.0f, 1.0f), math::Random(-1.0f, 1.0f)
            //         )
            //     );
            // }

            // {
            //     dynamic_cast<gfx::PointLigth*>(m_scene.GetLights().begin()->get())->MoveFor(
            //         math::VECTOR_LEFT * 0.5f + math::VECTOR_UP * 0.5f + math::VECTOR_BACKWARD * 0.1f
            //     );
            // }

            auto curr_frame = std::chrono::steady_clock::now();
            std::cout << "FPS: " << std::to_string(1.0f / std::chrono::duration<float>(curr_frame - m_last_frame).count()) << std::endl;
            m_last_frame = curr_frame;
        }
    }
}
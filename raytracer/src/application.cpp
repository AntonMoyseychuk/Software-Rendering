#include "application.hpp"
#include "objects/sphere.hpp"
#include "objects/point_light.hpp"

#include <iostream>

namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_scene(m_window), m_last_frame(std::chrono::steady_clock::now())
    {
        m_window->Init(title, width, height);
        m_window->SetBackgroundColor(gfx::Color(100).rgba);
        
        m_scene.SetAntialiasingLevel(gfx::AntialiasingLevel::X8);

        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, -100.5f, -1.0f), 100.0f, gfx::Material(gfx::Color::YELLOW, 1.5f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.0f, -1.0f), 0.5f, gfx::Material(gfx::Color::MAGENTA, 1.5f)));

        m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(5.0f, 5.0f, 5.0f), gfx::Color::WHITE, 1.0f));
    }
    
    void Application::Run() noexcept {
        while (m_scene.GetWindow()->IsOpen()) {
            m_scene.GetWindow()->PollEvent();

            // SDL_PumpEvents();
            // auto keyboard_state = SDL_GetKeyboardState(nullptr);
            // if (keyboard_state[SDL_Scancode::SDL_SCANCODE_A]) {
            //     light->MoveFor(math::VECTOR_LEFT * 0.1f);
            // }
            // if (keyboard_state[SDL_Scancode::SDL_SCANCODE_W]) {
            //     light->MoveFor(math::VECTOR_BACKWARD * 0.1f);
            // }
            // if (keyboard_state[SDL_Scancode::SDL_SCANCODE_D]) {
            //     light->MoveFor(math::VECTOR_RIGHT * 0.1f);
            // }
            // if (keyboard_state[SDL_Scancode::SDL_SCANCODE_S]) {
            //     light->MoveFor(math::VECTOR_FORWARD * 0.1f);
            // }
            // if (keyboard_state[SDL_Scancode::SDL_SCANCODE_UP]) {
            //     light->MoveFor(math::VECTOR_UP * 0.1f);
            // }
            // if (keyboard_state[SDL_Scancode::SDL_SCANCODE_DOWN]) {
            //     light->MoveFor(math::VECTOR_DOWN * 0.1f);
            // }

            m_scene.Render();
            m_scene.GetWindow()->PresentPixelBuffer();

            auto curr_frame = std::chrono::steady_clock::now();
            std::cout << "FPS: " << std::to_string(1.0f / std::chrono::duration<float>(curr_frame - m_last_frame).count()) << std::endl;
            m_last_frame = curr_frame;
        }
    }
}
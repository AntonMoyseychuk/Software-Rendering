#include "application.hpp"

#include "objects/triangle.hpp"
#include "objects/sphere.hpp"

#include "objects/point_light.hpp"
#include "objects/directional_light.hpp"
#include "objects/ambient_light.hpp"

#include <iostream>
#include <memory>

#ifdef _DEBUG
    #include <iostream>
    #define LOG_EXPR(expresion) std::cout << "{" << #expresion << "}: " << (expresion) << std::endl
    #define LOG(tag, expresion) std::cout << "{" << tag << "}: " << (expresion) << std::endl
#else 
    #define LOG_EXPR(expresion) (void)0
    #define LOG(tag, expresion) (void)0
#endif

namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_renderer(), m_scene(), 
            m_camera(math::vec3f(0.0f, 0.0f, 2.0f), math::VECTOR_BACKWARD, math::VECTOR_UP, 45.0f, (float)width / height),
                m_last_frame(std::chrono::steady_clock::now())
    {
        m_window->Init(title, width, height);
        m_window->SetBackgroundColor(gfx::Color(80).rgba);

        m_renderer.SetAntialiasingLevel(gfx::AntialiasingLevel::X2);
        m_renderer.SetReflectionDepth(2);

        m_camera.SetViewportSize(math::vec2ui(width, height) * static_cast<float>(m_renderer.GetAntialiasingLevel()));
        
        // for (std::size_t i = 0; i < 100; ++i) {
        //     m_scene.AddDrawble(std::make_shared<gfx::Sphere>(
        //             math::vec3f(math::Random(-9.0f, 9.0f), math::Random(-9.0f, 9.0f), math::Random(-9.0f, 9.0f)),
        //             math::Random(0.5f, 1.0f), 
        //             gfx::Material(gfx::Color(math::Random(0, 255), math::Random(0, 255), math::Random(0, 255)), math::Random(-500.0f, 500.0f), math::Random(0.0f, 0.8f))
        //         )
        //     );
        // }
        
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.0f, -3.0f), 0.4f, gfx::Material(gfx::Color::MAGENTA, 500)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-1.0f, 0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::RED, 500)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::GREEN, 500, 0.2f)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(1.0f, 0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::BLUE, 500, 0.4f)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-1.0f, -0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::YELLOW, 500, 0.6f)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, -0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::CYAN, 500, 0.8f)));
        // m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(1.0f, -0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::WHITE, 500, 1.0f)));
        m_scene.AddDrawble(std::make_shared<gfx::Triangle>(math::vec3f(-1.0f, -1.0f, 5.0f), math::vec3f(0.0f, 1.0f, 5.0f), math::vec3f(1.0f, -1.0f, 5.0f), 
            gfx::Material(gfx::Color::RED))
        );


        m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(math::vec3f(1.0f, -1.0f, -2.0f), gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(math::VECTOR_RIGHT, gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(8.0f, -10.0f, 8.0f), gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(-8.0f, -10.0f, 8.0f), gfx::Color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<gfx::AmbientLight>(gfx::Color::WHITE, 0.1f));
    }
    
    void Application::Run() noexcept {
        m_renderer.SetBackgroundColor(gfx::UInt32ToColor(m_window->GetBackgroundColor()));
        math::vec2ui last_window_size;

        while (m_window->IsOpen()) {
            const auto curr_frame = std::chrono::steady_clock::now();
            const auto dt = std::chrono::duration<float>(curr_frame - m_last_frame).count();
            m_last_frame = curr_frame;

            m_window->PollEvent();

            const math::vec2ui curr_window_size(m_window->GetWidth(), m_window->GetHeight());
            if (last_window_size != curr_window_size) {
                m_renderer.SetOutputFrameSize(curr_window_size);
                
                m_camera.SetAspectRatio((float)curr_window_size.x / curr_window_size.y);
                m_camera.SetViewportSize(curr_window_size * static_cast<float>(m_renderer.GetAntialiasingLevel()));

                last_window_size = curr_window_size;
            }

            // this->_UpdateLight(m_scene.GetLights().begin()->get(), dt);
            // this->_UpdateDrawable(m_scene.GetDrawables().begin()->get(), dt);
            this->_UpdateCamera(&m_camera, dt);
            
            const auto& frame = m_renderer.Render(m_scene, m_camera);
            m_window->FillPixelBuffer(frame);
            m_window->PresentPixelBuffer();          
            
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;
        }
    }
    
    void Application::_UpdateCamera(gfx::Camera* camera, float dt) noexcept {
        using namespace win_framewrk;
    
        if (camera != nullptr) {
            if (m_window->IsKeyPressed(Key::LALT)) {
                camera->Rotate(-math::ToRadians(180.0f), math::vec2f(0.0f, 1.0f));
            }

            if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                camera->Rotate(math::ToRadians(5.0f * dt), math::vec2f(0.0f, 1.0f));
            } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                camera->Rotate(-math::ToRadians(5.0f * dt), math::vec2f(0.0f, 1.0f));
            }

            if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                camera->Rotate(-math::ToRadians(5.0f * dt), math::vec2f(1.0f, 0.0f));
            } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                camera->Rotate(math::ToRadians(5.0f * dt), math::vec2f(1.0f, 0.0f));
            }

            if (m_window->IsKeyPressed(Key::W)) {
                camera->MoveFor(2.0f * camera->GetForward() * dt);
            } else if (m_window->IsKeyPressed(Key::S)) {
                camera->MoveFor(-2.0f * camera->GetForward() * dt);
            }

            if (m_window->IsKeyPressed(Key::A)) {
                camera->MoveFor(-2.0f * camera->GetRight() * dt);
            } else if (m_window->IsKeyPressed(Key::D)) {
                camera->MoveFor(2.0f * camera->GetRight() * dt);
            }

            if (m_window->IsKeyPressed(Key::LCTRL)) {
                camera->MoveFor(-2.0f * camera->GetUp() * dt);
            } else if (m_window->IsKeyPressed(Key::SPASE)) {
                camera->MoveFor(2.0f * camera->GetUp() * dt);
            }
        }
    }
    
    void Application::_UpdateDrawable(gfx::IDrawable* drawable, float dt) noexcept {
        using namespace win_framewrk;
    
        if (drawable != nullptr) {
            if (m_window->IsKeyPressed(Key::SPASE) == false) {
                if (m_window->IsKeyPressed(Key::W)) {
                    drawable->MoveFor(math::VECTOR_UP * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::S)) {
                    drawable->MoveFor(math::VECTOR_DOWN * 2.0f * dt);
                }
                
                if (m_window->IsKeyPressed(Key::A)) {
                    drawable->MoveFor(math::VECTOR_LEFT * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::D)) {
                    drawable->MoveFor(math::VECTOR_RIGHT * 2.0f * dt);
                }

                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    drawable->MoveFor(math::VECTOR_BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    drawable->MoveFor(math::VECTOR_FORWARD * 2.0f * dt);
                }
            }
        }
    }

    void Application::_UpdateLight(gfx::ILight* light, float dt) noexcept {
        using namespace win_framewrk;
        
        gfx::PointLigth* point_light = nullptr;
        if ((point_light = dynamic_cast<gfx::PointLigth*>(light)) != nullptr) {
            if (m_window->IsKeyPressed(Key::SPASE)) {
                if (m_window->IsKeyPressed(Key::W)) {
                    point_light->MoveFor(math::VECTOR_UP * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::S)) {
                    point_light->MoveFor(math::VECTOR_DOWN * 2.0f * dt);
                }

                if (m_window->IsKeyPressed(Key::A)) {
                    point_light->MoveFor(math::VECTOR_LEFT * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::D)) {
                    point_light->MoveFor(math::VECTOR_RIGHT * 2.0f * dt);
                }
                
                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    point_light->MoveFor(math::VECTOR_BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    point_light->MoveFor(math::VECTOR_FORWARD * 2.0f * dt);
                }
            }
        }
    }
}
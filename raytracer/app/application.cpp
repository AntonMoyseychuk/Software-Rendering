#include "application.hpp"

#include "graphics/drawables/triangle.hpp"
#include "graphics/drawables/sphere.hpp"

#include "graphics/materials/diffuse_material.hpp"
#include "graphics/materials/metal_material.hpp"
#include "graphics/materials/dielectric_material.hpp"

#include "graphics/light/point_light.hpp"
#include "graphics/light/directional_light.hpp"
#include "graphics/light/ambient_light.hpp"

#include <iostream>
#include <memory>

// #ifdef _DEBUG
//     #include <iostream>
//     #define LOG_EXPR(expresion) std::cout << "{" << #expresion << "}: " << (expresion) << std::endl
//     #define LOG(tag, expresion) std::cout << "{" << tag << "}: " << (expresion) << std::endl
// #else 
//     #define LOG_EXPR(expresion) (void)0
//     #define LOG(tag, expresion) (void)0
// #endif


static void _VertexShader(gfx::Triangle& triangle, const math::Quaternion& q) noexcept {
    triangle[0].position = triangle[0].position * q;
    triangle[1].position = triangle[1].position * q;
    triangle[2].position = triangle[2].position * q;
}

namespace raytracing {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_renderer(), m_scene(), 
            // m_camera(math::vec3f(0.0f), math::vec3f(0.0f, 0.0f, -5.0f), math::VECTOR_UP, 90.0f, (float)width / height),
                m_last_frame(std::chrono::steady_clock::now())
    {
        using namespace math;

        m_window->Init(title, width, height);

        m_renderer.SetBackgroundColor(gfx::Color::BLACK);
        m_renderer.SetAntialiasingLevel(gfx::AntialiasingLevel::X2);
        m_renderer.SetReflectionDepth(3);

        auto camera = std::make_shared<gfx::Camera>(math::vec3f(0.0f), math::vec3f(0.0f, 0.0f, -5.0f), math::VECTOR_UP, 90.0f, (float)width / height);
        camera->SetViewportSize(vec2ui(width, height) * static_cast<float>(m_renderer.GetAntialiasingLevel()));

        m_scene.SetCamera(camera);

        m_scene.AddDrawble(std::make_shared<gfx::Triangle>(
                gfx::Vertex(vec3f(-4.0f, -3.0f, -9.0f)), 
                gfx::Vertex(vec3f(0.0f, 5.0f, -9.0f)), 
                gfx::Vertex(vec3f(4.0f, -3.0f, -9.0f)),
                std::make_shared<gfx::Metal>(gfx::Color::GREY)
            )
        );

        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(0.0f, 0.0f, -5.5f), 1.6f, std::make_shared<gfx::Dielectric>(1.4f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(-3.0f, 0.65f, -5.0f), 0.8f, std::make_shared<gfx::Diffuse>(gfx::Color::RED)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(-1.5f, 0.65f, -5.0f), 0.8f, std::make_shared<gfx::Metal>(gfx::Color::GREEN, 0.0f, 500.0f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(0.0f, 0.65f, -5.0f), 0.8f, std::make_shared<gfx::Diffuse>(gfx::Color::BLUE)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(1.5f, 0.65f, -5.0f), 0.8f, std::make_shared<gfx::Metal>(gfx::Color::MAGENTA, 0.2f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(3.0f, 0.65f, -5.0f), 0.8f, std::make_shared<gfx::Diffuse>(gfx::Color::ORANGE)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(-3.0f, -1.0f, -5.0f), 0.8f, std::make_shared<gfx::Metal>(gfx::Color::YELLOW, 0.5f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(-1.5f, -1.0f, -5.0f), 0.8f, std::make_shared<gfx::Diffuse>(gfx::Color::PURPLE)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(0.0f, -1.0f, -5.0f), 0.8f, std::make_shared<gfx::Metal>(gfx::Color::CYAN)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(1.5f, -1.0f, -5.0f), 0.8f, std::make_shared<gfx::Diffuse>(gfx::Color::GOLDEN)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(vec3f(3.0f, -1.0f, -5.0f), 0.8f, std::make_shared<gfx::Metal>(gfx::Color::SKY_BLUE)));
        
        m_scene.AddDrawble(std::make_shared<gfx::Triangle>(
                gfx::Vertex(vec3f(-15.0f, -2.0f, 2.0f)), 
                gfx::Vertex(vec3f(0.0f, -1.0f, -17.0f)), 
                gfx::Vertex(vec3f(15.0f, -2.0f, 2.0f)),
                std::make_shared<gfx::Metal>(gfx::Color::BLACK, 0.05f)
            )
        );
        
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(-8.0f, -10.0f, 8.0f), gfx::Color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(vec3f(2.0f, -1.0f, -2.0f), gfx::Color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<gfx::AmbientLight>(gfx::Color::WHITE, 0.1f));
    }
    
    void Application::Run() noexcept {
        math::vec2ui last_window_size;

        auto& camera = m_scene.GetCamera();

        while (m_window->IsOpen()) {
            const auto curr_frame = std::chrono::steady_clock::now();
            const auto dt = std::chrono::duration<float>(curr_frame - m_last_frame).count();
            m_last_frame = curr_frame;

            m_window->PollEvent();

            const math::vec2ui curr_window_size(m_window->GetWidth(), m_window->GetHeight());
            if (last_window_size != curr_window_size) {
                m_renderer.SetOutputFrameSize(curr_window_size);
                
                camera->SetAspectRatio((float)curr_window_size.x / curr_window_size.y);
                camera->SetViewportSize(curr_window_size * static_cast<float>(m_renderer.GetAntialiasingLevel()));

                last_window_size = curr_window_size;
            }

            // this->_UpdateLight(m_scene.GetLights().begin()->get(), dt);
            this->_UpdateDrawable(m_scene.GetDrawables().begin()->get(), dt);
            this->_UpdateCamera(camera.get(), dt);
            
            const auto& frame = m_renderer.Render(m_scene);
            m_window->FillPixelBuffer(frame);
            m_window->PresentPixelBuffer();          
            
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;
        }
    }
    
    void Application::_UpdateCamera(gfx::Camera* camera, float dt) noexcept {
        using namespace win_framewrk;
    
        if (camera != nullptr) {
            if (m_window->IsKeyPressed(Key::LALT)) {
                if (m_window->IsKeyPressed(Key::RALT)) {
                    camera->Rotate(math::ToRadians(180.0f), math::vec2f(0.0f, 1.0f));
                }

                if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                    camera->Rotate(math::ToRadians(-25.0f * dt), math::vec2f(0.0f, 1.0f));
                } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                    camera->Rotate(math::ToRadians(25.0f * dt), math::vec2f(0.0f, 1.0f));
                }

                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    camera->Rotate(math::ToRadians(-25.0f * dt), math::vec2f(1.0f, 0.0f));
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    camera->Rotate(math::ToRadians(25.0f * dt), math::vec2f(1.0f, 0.0f));
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
    }
    
    void Application::_UpdateDrawable(gfx::IDrawable* drawable, float dt) noexcept {
        using namespace win_framewrk;
        using namespace math;
    
        if (drawable != nullptr) {
            if (m_window->IsKeyPressed(Key::SPASE)) {
                if (m_window->IsKeyPressed(Key::W)) {
                    drawable->MoveFor(VECTOR_UP * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::S)) {
                    drawable->MoveFor(VECTOR_DOWN * 2.0f * dt);
                }
                
                if (m_window->IsKeyPressed(Key::A)) {
                    drawable->MoveFor(VECTOR_LEFT * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::D)) {
                    drawable->MoveFor(VECTOR_RIGHT * 2.0f * dt);
                }

                if (m_window->IsKeyPressed(Key::Z)) {
                    drawable->MoveFor(VECTOR_BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::X)) {
                    drawable->MoveFor(VECTOR_FORWARD * 2.0f * dt);
                }
                
                if (dynamic_cast<gfx::Triangle*>(drawable) != nullptr) {
                    if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            Quaternion(cosf(MATH_PI / 360.0f), sinf(MATH_PI / 360.0f), 0.0f, 0.0f)
                        );
                    } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            Quaternion(cosf(MATH_PI / 360.0f), sinf(-MATH_PI / 360.0f), 0.0f, 0.0f)
                        );
                    }

                    if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            Quaternion(cosf(MATH_PI / 360.0f), 0.0, sinf(-MATH_PI / 360.0f), 0.0f)
                        );
                    } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            Quaternion(cosf(MATH_PI / 360.0f), 0.0f, sinf(MATH_PI / 360.0f), 0.0f)
                        );
                    }
                }
            }
        }
    }

    void Application::_UpdateLight(gfx::ILight* light, float dt) noexcept {
        using namespace win_framewrk;
        
        gfx::PointLigth* point_light = nullptr;
        if ((point_light = dynamic_cast<gfx::PointLigth*>(light)) != nullptr) {
            if (m_window->IsKeyPressed(Key::LSHIFT)) {
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

                if (m_window->IsKeyPressed(Key::Z)) {
                    point_light->MoveFor(math::VECTOR_BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::X)) {
                    point_light->MoveFor(math::VECTOR_FORWARD * 2.0f * dt);
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
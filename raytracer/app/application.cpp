#include "application.hpp"

#include "graphics/drawables/triangle.hpp"
#include "graphics/drawables/sphere.hpp"

#include "graphics/materials/diffuse_material.hpp"
#include "graphics/materials/metal_material.hpp"
#include "graphics/materials/dielectric_material.hpp"

#include "graphics/light/point_light.hpp"
#include "graphics/light/directional_light.hpp"
#include "graphics/light/ambient_light.hpp"

#include "math_3d/math.hpp"

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


static void _VertexShader(raytracing::gfx::Triangle& triangle, const math::quaternion& q) noexcept {
    triangle[0].position = triangle[0].position * q;
    triangle[1].position = triangle[1].position * q;
    triangle[2].position = triangle[2].position * q;
}

namespace raytracing::app {
    Application::Application(const std::string &title, uint32_t width, uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_renderer(), m_scene(), m_last_frame(std::chrono::steady_clock::now())
    {
        using namespace math;
        using namespace raytracing::gfx;

        m_window->Init(title, width, height);

        m_renderer.SetBackgroundColor(color::BLACK);
        m_renderer.SetAntialiasingLevel(AntialiasingLevel::X4);
        m_renderer.SetReflectionDepth(3);

        auto camera = std::make_shared<Camera>(vec3f(0.0f), vec3f(0.0f, 0.0f, -5.0f), vec3f::UP, 90.0f, (float)width / height);
        camera->SetViewportSize(vec2f(width, height) * static_cast<float>(m_renderer.GetAntialiasingLevel()));

        m_scene.SetCamera(camera);

        m_scene.AddDrawble(std::make_shared<Triangle>(
                Vertex(vec3f(-4.0f, -3.0f, -9.0f)), 
                Vertex(vec3f(0.0f, 5.0f, -9.0f)), 
                Vertex(vec3f(4.0f, -3.0f, -9.0f)),
                std::make_shared<Metal>(color::GREY)
            )
        );

        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(0.0f, 0.0f, -5.5f), 1.6f, std::make_shared<Dielectric>(1.4f)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(-3.0f, 0.65f, -5.0f), 0.8f, std::make_shared<Diffuse>(color::RED)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(-1.5f, 0.65f, -5.0f), 0.8f, std::make_shared<Metal>(color::GREEN, 0.0f, 500.0f)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(0.0f, 0.65f, -5.0f), 0.8f, std::make_shared<Diffuse>(color::BLUE)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(1.5f, 0.65f, -5.0f), 0.8f, std::make_shared<Metal>(color::MAGENTA, 0.2f)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(3.0f, 0.65f, -5.0f), 0.8f, std::make_shared<Diffuse>(color::ORANGE)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(-3.0f, -1.0f, -5.0f), 0.8f, std::make_shared<Metal>(color::YELLOW, 0.5f)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(-1.5f, -1.0f, -5.0f), 0.8f, std::make_shared<Diffuse>(color::PURPLE)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(0.0f, -1.0f, -5.0f), 0.8f, std::make_shared<Metal>(color::CYAN)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(1.5f, -1.0f, -5.0f), 0.8f, std::make_shared<Diffuse>(color::GOLDEN)));
        m_scene.AddDrawble(std::make_shared<Sphere>(vec3f(3.0f, -1.0f, -5.0f), 0.8f, std::make_shared<Metal>(color::SKY_BLUE)));
        
        m_scene.AddDrawble(std::make_shared<Triangle>(
                Vertex(vec3f(-15.0f, -2.0f, 2.0f)), 
                Vertex(vec3f(0.0f, -1.0f, -17.0f)), 
                Vertex(vec3f(15.0f, -2.0f, 2.0f)),
                std::make_shared<Metal>(color::BLACK, 0.05f)
            )
        );
        
        // m_scene.AddLight(std::make_shared<PointLigth>(math::vec3f(-8.0f, -10.0f, 8.0f), Color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<DirectionalLigth>(vec3f(2.0f, -1.0f, -2.0f), color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<AmbientLight>(color::WHITE, 0.1f));
    }
    
    void Application::Run() noexcept {
        math::vec2f last_window_size;

        auto& camera = m_scene.GetCamera();

        while (m_window->IsOpen()) {
            const auto curr_frame = std::chrono::steady_clock::now();
            const auto dt = std::chrono::duration<float>(curr_frame - m_last_frame).count();
            m_last_frame = curr_frame;

            m_window->PollEvent();

            const math::vec2f curr_window_size(m_window->GetWidth(), m_window->GetHeight());
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
                    camera->Rotate(math::to_radians(180.0f), math::vec2f(0.0f, 1.0f));
                }

                if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                    camera->Rotate(math::to_radians(-25.0f * dt), math::vec2f(0.0f, 1.0f));
                } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                    camera->Rotate(math::to_radians(25.0f * dt), math::vec2f(0.0f, 1.0f));
                }

                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    camera->Rotate(math::to_radians(-25.0f * dt), math::vec2f(1.0f, 0.0f));
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    camera->Rotate(math::to_radians(25.0f * dt), math::vec2f(1.0f, 0.0f));
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
                    drawable->MoveFor(vec3f::UP * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::S)) {
                    drawable->MoveFor(vec3f::DOWN * 2.0f * dt);
                }
                
                if (m_window->IsKeyPressed(Key::A)) {
                    drawable->MoveFor(vec3f::LEFT * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::D)) {
                    drawable->MoveFor(vec3f::RIGHT * 2.0f * dt);
                }

                if (m_window->IsKeyPressed(Key::Z)) {
                    drawable->MoveFor(vec3f::BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::X)) {
                    drawable->MoveFor(vec3f::FORWARD * 2.0f * dt);
                }
                
                if (dynamic_cast<gfx::Triangle*>(drawable) != nullptr) {
                    if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            quaternion(cosf(MATH_PI / 360.0f), sinf(MATH_PI / 360.0f), 0.0f, 0.0f)
                        );
                    } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            quaternion(cosf(MATH_PI / 360.0f), sinf(-MATH_PI / 360.0f), 0.0f, 0.0f)
                        );
                    }

                    if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            quaternion(cosf(MATH_PI / 360.0f), 0.0, sinf(-MATH_PI / 360.0f), 0.0f)
                        );
                    } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                        _VertexShader(*dynamic_cast<gfx::Triangle*>(drawable), 
                            quaternion(cosf(MATH_PI / 360.0f), 0.0f, sinf(MATH_PI / 360.0f), 0.0f)
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
                    point_light->MoveFor(math::vec3f::UP * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::S)) {
                    point_light->MoveFor(math::vec3f::DOWN * 2.0f * dt);
                }

                if (m_window->IsKeyPressed(Key::A)) {
                    point_light->MoveFor(math::vec3f::LEFT * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::D)) {
                    point_light->MoveFor(math::vec3f::RIGHT * 2.0f * dt);
                }

                if (m_window->IsKeyPressed(Key::Z)) {
                    point_light->MoveFor(math::vec3f::BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::X)) {
                    point_light->MoveFor(math::vec3f::FORWARD * 2.0f * dt);
                }
                
                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    point_light->MoveFor(math::vec3f::BACKWARD * 2.0f * dt);
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    point_light->MoveFor(math::vec3f::FORWARD * 2.0f * dt);
                }
            }
        }
    }
}
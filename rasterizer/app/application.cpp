#include "application.hpp"
#include "math_3d/math.hpp"

#include "graphics/core_engine.hpp"
#include "graphics/model.hpp"

#include <iostream>
#include <memory>
#include <cassert>


namespace rasterization {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        m_window->Init(title, width, height);

        m_rasterizer.BindWindow(m_window);
        assert(m_rasterizer.IsWindowBinded() != nullptr);
    }

    void Application::Run() noexcept {
        using namespace gfx;
        using namespace math;
        using namespace win_framewrk;

        static const CoreEngine& core = CoreEngine::Get();

        Model model("..\\..\\..\\rasterizer\\assets\\human.obj");
        
        size_t vbo = core.CreateBuffer(BufferType::VERTEX, model.vertexes.data(), model.vertexes.size() * sizeof(model.vertexes[0]));
        size_t ibo = core.CreateIndexBuffer(model.vert_indexes.data(), model.vert_indexes.size());
        model.Free();

        core.SetShaderUniform("scale", scale(mat4f::IDENTITY, vec3f(0.5f)));

        mat4f rotation, translation;
        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            const float angle = to_radians(dt) * 20.0f;

        #pragma region input
            if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                rotation = rotate_y(rotation, -angle);
                core.SetShaderUniform("rotate", rotation);
            } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                rotation = rotate_y(rotation, angle);
                core.SetShaderUniform("rotate", rotation);
            }

            if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                rotation = rotate_x(rotation, -angle);
                core.SetShaderUniform("rotate", rotation);
            } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                rotation = rotate_x(rotation, angle);
                core.SetShaderUniform("rotate", rotation);
            }

            if (m_window->IsKeyPressed(Key::D)) {
                translation = translate(translation, vec3f::RIGHT * dt);
                core.SetShaderUniform("translate", translation);
            } else if (m_window->IsKeyPressed(Key::A)) {
                translation = translate(translation, vec3f::LEFT * dt);
                core.SetShaderUniform("translate", translation);
            }

            if (m_window->IsKeyPressed(Key::W)) {
                translation = translate(translation, vec3f::UP * dt);
                core.SetShaderUniform("translate", translation);
            } else if (m_window->IsKeyPressed(Key::S)) {
                translation = translate(translation, vec3f::DOWN * dt);
                core.SetShaderUniform("translate", translation);
            }
        #pragma endregion input

            m_rasterizer.Render(RenderMode::TRIANGLES, vbo, ibo, color::GOLDEN);
            m_rasterizer.SwapBuffers(); 
            m_rasterizer.ClearBackBuffer();
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
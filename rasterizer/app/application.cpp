#include "application.hpp"
#include "math_3d/math.hpp"

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

        Model model("..\\..\\..\\rasterizer\\assets\\human.obj");
        
        size_t vbo = m_rasterizer.CreateBuffer(BufferType::VERTEX, model.vertexes.data(), model.vertexes.size() * sizeof(model.vertexes[0]));
        size_t ibo = m_rasterizer.CreateBuffer(BufferType::INDEX, model.indexes.data(), model.indexes.size() * sizeof(model.indexes[0]));
        model.Free();

        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            const float angle = to_radians(dt) * 10.0f;

        #pragma region input
            auto& internal_buff = m_rasterizer.GetVertexBuffer(vbo);
            if (m_window->IsKeyPressed(Key::D)) {
                for (auto& vert : internal_buff) {
                    vert *= quaternion(cosf(angle), 0.0f, sinf(angle), 0.0f);
                }
            } else if (m_window->IsKeyPressed(Key::A)) {
                for (auto& vert : internal_buff) {
                    vert *= quaternion(cosf(-angle), 0.0f, sinf(-angle), 0.0f);
                }
            }

            if (m_window->IsKeyPressed(Key::W)) {
                for (auto& vert : internal_buff) {
                    vert *= quaternion(cosf(-angle), sinf(-angle), 0.0f, 0.0f);
                }
            } else if (m_window->IsKeyPressed(Key::S)) {
                for (auto& vert : internal_buff) {
                    vert *= quaternion(cosf(angle), sinf(angle), 0.0f, 0.0f);
                }
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
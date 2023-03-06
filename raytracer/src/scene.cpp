#include "scene.hpp"

namespace app {
    Scene::Scene(win_framewrk::Window *window)
        : m_window(window), m_buffer(window->GetWidth() * window->GetHeight()), m_drawables()
    {
    }
    
    void Scene::Render() const noexcept {
        const auto FOV = tanf(3.1415f / 4.0f / 2.f);
        const auto WIDTH = m_window->GetWidth();
        const auto HEIGHT = m_window->GetHeight();
        const auto CAMERA_POS = math::vec4f(0);
        for (const auto& drawable : m_drawables) {
            drawable->Render(m_buffer, WIDTH, HEIGHT, CAMERA_POS, math::VECTOR_BACKWARD, FOV);
        }
        m_window->FillPixelBuffer(m_buffer);
    }
    
    void Scene::SetWindow(win_framewrk::Window *window) noexcept {
        assert(window != nullptr);
        m_window = window;
        m_buffer.resize(window->GetWidth() * window->GetHeight());
    }

    void Scene::AddDrawble(std::shared_ptr<gfx::Drawable> drawable) noexcept {
        m_drawables.push_back(drawable);
    }
}
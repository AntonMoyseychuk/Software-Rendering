#include "scene.hpp"

namespace app {
    Scene::Scene(win_framewrk::Window *window)
        : m_window(window), m_drawables()
    {
    }
    
    void Scene::Render() const noexcept {
        for (const auto& drawable : m_drawables) {
            //auto buffer = drawable->Render();
        }
    }
}
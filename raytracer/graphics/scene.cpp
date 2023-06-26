#include "scene.hpp"

namespace raytracing::gfx {
    void Scene::AddDrawble(std::shared_ptr<IDrawable> drawable) noexcept {
        m_drawables.push_back(drawable);
    }

    const std::list<std::shared_ptr<IDrawable>> &Scene::GetDrawables() const noexcept {
        return m_drawables;
    }

    std::list<std::shared_ptr<IDrawable>> &Scene::GetDrawables() noexcept {
        return m_drawables;
    }

    void Scene::AddLight(std::shared_ptr<ILight> light) noexcept {
        m_lights.push_back(light);
    }
    
    const std::list<std::shared_ptr<ILight>> &Scene::GetLights() const noexcept {
        return m_lights;
    }
    
    std::list<std::shared_ptr<ILight>> &Scene::GetLights() noexcept {
        return m_lights;
    }
    
    void Scene::SetCamera(std::shared_ptr<Camera> camera) noexcept {
        m_camera_ptr = camera;
    }
    
    const std::shared_ptr<Camera>& Scene::GetCamera() const noexcept {
        return m_camera_ptr;
    }
    
    std::shared_ptr<Camera>& Scene::GetCamera() noexcept {
        return m_camera_ptr;
    }
}
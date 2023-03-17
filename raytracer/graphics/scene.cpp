#include "scene.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    void Scene::AddDrawble(std::shared_ptr<gfx::IDrawable> drawable) noexcept {
        m_drawables.push_back(drawable);
    }

    const std::list<std::shared_ptr<gfx::IDrawable>> &Scene::GetDrawables() const noexcept {
        return m_drawables;
    }

    std::list<std::shared_ptr<gfx::IDrawable>> &Scene::GetDrawables() noexcept {
        return m_drawables;
    }

    void Scene::AddLight(std::shared_ptr<gfx::ILight> light) noexcept {
        m_lights.push_back(light);
    }
    
    const std::list<std::shared_ptr<gfx::ILight>> &Scene::GetLights() const noexcept {
        return m_lights;
    }
    
    std::list<std::shared_ptr<gfx::ILight>> &Scene::GetLights() noexcept {
        return m_lights;
    }
}
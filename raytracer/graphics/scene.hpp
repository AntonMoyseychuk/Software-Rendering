#pragma once
#include "graphics/drawables/drawable.hpp"
#include "graphics/light/light.hpp"

#include "camera.hpp"

#include <list>
#include <memory>

namespace gfx {
    class Scene {
    public:
        Scene() = default;
        
    #pragma region getters-setters
        void AddDrawble(std::shared_ptr<IDrawable> drawable) noexcept;
        const std::list<std::shared_ptr<IDrawable>>& GetDrawables() const noexcept;
        std::list<std::shared_ptr<IDrawable>>& GetDrawables() noexcept;

        void AddLight(std::shared_ptr<ILight> light) noexcept;
        const std::list<std::shared_ptr<ILight>>& GetLights() const noexcept;
        std::list<std::shared_ptr<ILight>>& GetLights() noexcept;

        void SetCamera(std::shared_ptr<Camera> camera) noexcept;
        const std::shared_ptr<Camera>& GetCamera() const noexcept;
        std::shared_ptr<Camera>& GetCamera() noexcept;
    #pragma endregion getters-setters

    private:
        std::list<std::shared_ptr<IDrawable>> m_drawables;
        std::list<std::shared_ptr<ILight>> m_lights;
        std::shared_ptr<Camera> m_camera_ptr = nullptr;
    };
}
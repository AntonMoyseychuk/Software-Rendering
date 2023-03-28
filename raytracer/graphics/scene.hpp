#pragma once
#include "objects/drawable.hpp"
#include "objects/point_light.hpp"

#include <list>
#include <memory>

namespace gfx {
    class Scene {
    public:
        Scene() = default;
        
    #pragma region getters-setters
        void AddDrawble(std::shared_ptr<gfx::IDrawable> drawable) noexcept;
        const std::list<std::shared_ptr<gfx::IDrawable>>& GetDrawables() const noexcept;
        std::list<std::shared_ptr<gfx::IDrawable>>& GetDrawables() noexcept;

        void AddLight(std::shared_ptr<gfx::ILight> light) noexcept;
        const std::list<std::shared_ptr<gfx::ILight>>& GetLights() const noexcept;
        std::list<std::shared_ptr<gfx::ILight>>& GetLights() noexcept;
    #pragma endregion getters-setters

    private:
        std::list<std::shared_ptr<gfx::IDrawable>> m_drawables;
        std::list<std::shared_ptr<gfx::ILight>> m_lights;
    };
}
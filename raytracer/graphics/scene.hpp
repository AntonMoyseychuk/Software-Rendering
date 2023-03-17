#pragma once
#include "window_framework/window.hpp"
#include "objects/drawable.hpp"
#include "objects/point_light.hpp"

#include <list>
#include <memory>

namespace gfx {
    class Scene {
    public:
        Scene() = default;
        
        void AddDrawble(std::shared_ptr<gfx::IDrawable> drawable) noexcept;
        const std::list<std::shared_ptr<gfx::IDrawable>>& GetDrawables() const noexcept;
        std::list<std::shared_ptr<gfx::IDrawable>>& GetDrawables() noexcept;

        void AddLight(std::shared_ptr<gfx::ILight> light) noexcept;
        const std::list<std::shared_ptr<gfx::ILight>>& GetLights() const noexcept;
        std::list<std::shared_ptr<gfx::ILight>>& GetLights() noexcept;

    private:
        std::list<std::shared_ptr<gfx::IDrawable>> m_drawables;
        std::list<std::shared_ptr<gfx::ILight>> m_lights;
    };
}
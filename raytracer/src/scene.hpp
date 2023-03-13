#pragma once
#include "window_framework/window.hpp"
#include "objects/drawable.hpp"
#include "objects/point_light.hpp"

#include <list>
#include <memory>

namespace app {
    class Scene {
    public:
        Scene() = default;
        Scene(win_framewrk::Window* window);

        void Render() const noexcept;

        void SetWindow(win_framewrk::Window* window) noexcept;
        win_framewrk::Window* GetWindow() noexcept;
        const win_framewrk::Window* GetWindow() const noexcept;
        
        void AddDrawble(std::shared_ptr<gfx::IDrawable> drawable) noexcept;
        void AddLight(std::shared_ptr<gfx::ILight> light) noexcept;

    private:
        win_framewrk::Window* m_window = nullptr;
        std::list<std::shared_ptr<gfx::IDrawable>> m_drawables;
        std::list<std::shared_ptr<gfx::ILight>> m_lights;
    };
}
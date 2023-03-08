#pragma once
#include "window_framework/window.hpp"
#include "objects/drawable.hpp"

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

    private:
        win_framewrk::Window* m_window = nullptr;
        // mutable std::vector<std::uint32_t> m_buffer;
        std::list<std::shared_ptr<gfx::IDrawable>> m_drawables;
    };
}
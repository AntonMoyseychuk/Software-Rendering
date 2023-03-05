#pragma once
#include "window_framework/window.hpp"
#include "objects/drawable.hpp"
#include "graphics/camera.hpp"

#include <vector>
#include <memory>

namespace app {
    class Scene {
    public:
        Scene() = default;
        Scene(win_framewrk::Window* window);

        void Render() const noexcept;

    private:
        win_framewrk::Window* m_window = nullptr;
        std::vector<std::shared_ptr<gfx::Drawable>> m_drawables;
        std::vector<gfx::Camera> m_cameras;
    };
}
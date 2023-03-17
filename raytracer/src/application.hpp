#pragma once
#include "window_framework/window.hpp"

#include "graphics/renderer.hpp"
#include "graphics/scene.hpp"

#include "objects/camera.hpp"

#include <string>
#include <chrono>

namespace app {
    class Application {
    public:
        Application(const std::string& title, std::uint32_t width, std::uint32_t height);
        void Run() noexcept;

    private:
        win_framewrk::Window* m_window;
        gfx::Renderer m_renderer;
        gfx::Scene m_scene;
        gfx::Camera m_camera;

        std::chrono::steady_clock::time_point m_last_frame;
    };
}
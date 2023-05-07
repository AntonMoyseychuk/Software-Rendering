#pragma once
#include "window/window.hpp"

#include "graphics/renderer.hpp"

#include <string>
#include <chrono>

namespace raytracing {
    class Application {
    public:
        Application(const std::string& title, std::uint32_t width, std::uint32_t height);
        void Run() noexcept;

    private:
        void _UpdateCamera(gfx::Camera* camera, float dt) noexcept;
        void _UpdateDrawable(gfx::IDrawable* drawable, float dt) noexcept;
        void _UpdateLight(gfx::ILight* light, float dt) noexcept;

    private:
        win_framewrk::Window* m_window;
        gfx::Renderer m_renderer;
        gfx::Scene m_scene;

        std::chrono::steady_clock::time_point m_last_frame;
    };
}
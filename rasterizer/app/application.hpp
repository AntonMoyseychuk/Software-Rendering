#pragma once
#include "window/window.hpp"
#include "graphics/rasterizer.hpp"

#include <string>
#include <chrono>

namespace rasterization {
    class Application {
    public:
        Application(const std::string& title, std::uint32_t width, std::uint32_t height, size_t fps_lock = 60);
        void Run() noexcept;

        void SetFPSLock(size_t fps) const noexcept;

    private:
        float _LockFPS() const noexcept;

    private:
        win_framewrk::Window* m_window;
        gfx::Rasterizer m_rasterizer;

        using vbo_t = size_t;
        using ibo_t = size_t;
        std::unordered_map<std::string, std::pair<vbo_t, ibo_t>> m_VBO_IBO;

        mutable std::chrono::steady_clock::time_point m_last_frame;
        mutable float m_fps_lock;
    };
}
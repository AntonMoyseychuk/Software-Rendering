#pragma once
#include "window/window.hpp"

#include "math_3d/vec_operations.hpp"
#include "math_3d/mat_operations.hpp"

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

        struct Object {
            size_t vbo;
            size_t ibo;
        };
        struct Transform {
            math::mat4f translation;
            math::mat4f rotation;
            math::mat4f scale;
        } transform;
        std::unordered_map<std::string, Object> m_objects;

        math::vec3f m_camera_position;
        math::mat4f m_view_matrix;

        math::mat4f m_proj_matrix;

        math::vec3f m_light_position;

        size_t m_simple_shader = 0;
        size_t m_gouraud_shader = 0;

        mutable std::chrono::steady_clock::time_point m_last_frame;
        mutable float m_fps_lock;
    };
}
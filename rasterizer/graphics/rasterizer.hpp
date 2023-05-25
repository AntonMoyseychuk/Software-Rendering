#pragma once
#include "window/window.hpp"
#include "core/gl_api.hpp"
#include "thread_pool/thread_pool.hpp"

#include "math_3d/vec4.hpp"


namespace rasterization::gfx {
    enum class RenderMode : uint8_t { LINES, POINTS, TRIANGLES };

    class Rasterizer final {
    public:
        Rasterizer();
        Rasterizer(win_framewrk::Window* window);
        
        bool BindWindow(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* IsWindowBinded() const noexcept;

        void Render(RenderMode mode, size_t vbo_id, size_t ibo_id) const noexcept;
        void SwapBuffers() const noexcept;
        void ClearBackBuffer() const noexcept;

        void SetBackgroundColor(const math::color& color) noexcept;
        const math::color& GetBackgroundColor() const noexcept;

    private:
        void _ResizeZBuffer(uint32_t width, uint32_t height) const noexcept;

    private:
        void _VertexShader(const math::vec3f& local_coord, math::vec3f& screen_coord) const noexcept;
        void _Rasterize(const std::vector<math::vec3f> &screen_coords, std::vector<math::vec3f> &raster_coords) const noexcept;

        void _PointPixelShader(const math::vec3f& raster_coord, const math::color& color) const noexcept;
        void _LinePixelShader(const math::vec3f& raster_coord_v0, const math::vec3f& raster_coord_v1) const noexcept;
        void _TrianglePixelShader(const math::vec3f &raster_coord_0, const math::vec3f &raster_coord_1, const math::vec3f &raster_coord_2) const noexcept;

    private:
        mutable std::vector<float> m_z_buffer;

        // mutable util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };

        win_framewrk::Window* m_window_ptr = nullptr;
        const GLApi& m_core;

        math::color m_background_color = math::color::BLACK;
    };
}
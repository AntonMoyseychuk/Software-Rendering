#pragma once
#include "window/window.hpp"
#include "math_3d/color.hpp"

#include "model.hpp"
#include "thread_pool/thread_pool.hpp"

#include <vector>

namespace rasterization::gfx {
    enum class RenderMode : uint8_t { LINES, POINTS, TRIANGLES };
    enum class BufferType : uint8_t { VERTEX, INDEX };

    class Rasterizer final {
    public:
        Rasterizer() = default;
        Rasterizer(win_framewrk::Window* window);
        
        bool BindWindow(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* IsWindowBinded() const noexcept;

        void Render(RenderMode mode, size_t vbo_id, size_t ibo_id, math::Color color) const noexcept;
        void SwapBuffers() const noexcept;
        void ClearBackBuffer() const noexcept;

        void SetBackgroundColor(math::Color color) noexcept;
        math::Color GetBackgroundColor() const noexcept;

    public:
        size_t CreateBuffer(BufferType type, const void* buffer, size_t size) noexcept;
        std::vector<math::vec3f>& GetVertexBuffer(size_t id) noexcept;

    private:
        void _ResizeZBuffer(uint32_t width, uint32_t height) const noexcept;
        size_t _CreateVertexBuffer(const void* buffer, size_t size) noexcept;
        size_t _CreateIndexBuffer(const void* buffer, size_t count) noexcept;

    private:
        void _VertexShader(const math::vec3f& local_coord, math::vec3f& transformed_coord) const noexcept;
        void _Rasterize(const std::vector<math::vec3f> &transformed_coords, std::vector<math::vec3f> &screen_coords) const noexcept;

        void _PointPixelShader(const math::vec3f& screen_coord, math::Color color) const noexcept;
        void _LinePixelShader(const math::vec3f& screen_coord_v0, const math::vec3f& screen_coord_v1, math::Color color) const noexcept;
        void _TrianglePixelShader(
            const math::vec3f& local_coord0, 
            const math::vec3f& local_coord1, 
            const math::vec3f& local_coord2, 
            const math::vec3f& screen_coords0, 
            const math::vec3f& screen_coords1, 
            const math::vec3f& screen_coords2, 
            math::Color color,
            const math::vec3f& light_direction
        ) const noexcept;

    private:
        mutable std::vector<float> m_z_buffer;

        std::unordered_map<size_t, std::vector<math::vec3f>> m_vbos;
        std::unordered_map<size_t, std::vector<size_t>> m_ibos;

        // mutable util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };

        win_framewrk::Window* m_window_ptr = nullptr;

        math::Color m_background_color = math::Color::BLACK;
    };
}
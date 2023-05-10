#pragma once
#include "window/window.hpp"
#include "math_3d/color.hpp"

#include "model.hpp"

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

        void SetBackgroundColor(math::Color color) noexcept;
        math::Color GetBackgroundColor() const noexcept;

    public:
        size_t CreateBuffer(BufferType type, const void* buffer, size_t size) noexcept;
        std::vector<math::vec3f>& GetVertexBuffer(size_t id) noexcept;

    private:
        void _VertexShader(std::vector<math::vec3i> &out_coords, size_t vbo_id, std::shared_ptr<math::mat4f> mvp = nullptr) const noexcept;

    private:
        void _RenderPoint(const math::vec3i& point, math::Color color) const noexcept;

        static std::vector<int32_t>& _Interpolate(int32_t i0, int32_t d0, int32_t i1, int32_t d1, std::vector<int32_t>& values) noexcept;
        void _RenderLine(const math::vec3i& v0, const math::vec3i& v1, math::Color color) const noexcept;
        
        void _RenderTriangle(const math::vec3i& v0, const math::vec3i& v1, const math::vec3i& v2, 
            math::Color color) const noexcept;

    private:
        size_t _CreateVertexBuffer(const void* buffer, size_t size) noexcept;
        size_t _CreateIndexBuffer(const void* buffer, size_t count) noexcept;

    private:
        std::unordered_map<size_t, std::vector<math::vec3f>> m_vbos;
        std::unordered_map<size_t, std::vector<size_t>> m_ibos;

        win_framewrk::Window* m_window_ptr = nullptr;

        math::Color m_background_color = math::Color::BLACK;
    };
}
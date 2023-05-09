#pragma once
#include "window/window.hpp"
#include "math_3d/color.hpp"

#include "model.hpp"

#include <vector>

namespace rasterization::gfx {
    enum class RenderMode : uint8_t { LINES, POINTS, TRIANGLES };

    class Rasterizer final {
    public:
        Rasterizer() = default;
        Rasterizer(win_framewrk::Window* window);
        
        void Render(RenderMode mode, size_t vbo_id, size_t ibo_id, math::Color color) const noexcept;

        void BindWindow(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* IsWindowBinded() const noexcept;

        size_t AddVertexBuffer(const std::vector<math::vec3f>& vbo) noexcept;
        std::vector<math::vec3f>* GetVertexBuffer(size_t id) noexcept;
        
        size_t AddIndexBuffer(const std::vector<size_t>& ibo) noexcept;

    private:
        void _RenderPoint(const math::vec2i& _point, math::Color _color) const noexcept;

        static void _Interpolate(int32_t _i0, int32_t _d0, int32_t _i1, int32_t _d1, std::vector<int32_t>& _values) noexcept;
        void _RenderLine(const math::vec2i& _v0, const math::vec2i& _v1, math::Color _color) const noexcept;
        
        void _RenderTriangle(
            const math::vec2i& _v0, 
            const math::vec2i& _v1, 
            const math::vec2i& _v2, 
            math::Color color
        ) const noexcept;

    private:
        win_framewrk::Window* m_window_ptr = nullptr;

        std::unordered_map<size_t, std::vector<math::vec3f>> m_vbos;
        std::unordered_map<size_t, std::vector<size_t>> m_ibos;
    };
}
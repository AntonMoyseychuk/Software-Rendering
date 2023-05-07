#pragma once
#include "window/window.hpp"
#include "math_3d/color.hpp"

#include "model.hpp"

#include <vector>

namespace rasterization::gfx {
    enum class RenderMode : uint8_t { LINES };

    class Rasterizer final {
    public:
        Rasterizer() = default;
        Rasterizer(win_framewrk::Window* window);

        void BindWindow(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* IsWindowBinded() const noexcept;

        void Render(RenderMode mode, const Model& model, math::Color color) const noexcept;

    private:
        static void _Interpolate(int32_t i0, int32_t d0, int32_t i1, int32_t d1, std::vector<int32_t>& values) noexcept;
        void _RenderLine(const math::vec2i& _v0, const math::vec2i& _v1, math::Color color) const noexcept;
        void _RenderWireframeTriangle(
            const math::vec2i& _v0, 
            const math::vec2i& _v1, 
            const math::vec2i& _v2, 
            math::Color color
        ) const noexcept;

    private:
        win_framewrk::Window* m_window_ptr = nullptr;
    };
}
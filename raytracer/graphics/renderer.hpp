#pragma once
#include "graphics/scene.hpp"

#include "math_3d/vec4.hpp"

#include "thread_pool/thread_pool.hpp"

#include <vector>
        

namespace raytracing::gfx {
    enum class AntialiasingLevel { NONE = 1, X2 = 2, X4 = 4, X8 = 8, X12 = 12, X16 = 16, X32 = 32 };

    class Renderer {
    private:
        union _InternalColor {
            _InternalColor() = default;
            _InternalColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
                : r(_r), g(_g), b(_b), a(_a) {}

            explicit _InternalColor(uint32_t _rgba)
                : rgba(_rgba) {}

            struct {
                uint8_t r, g, b, a;
            };

            uint32_t rgba;
        };

    public:
        Renderer() = default;

        const std::vector<uint32_t>& Render(const Scene& scene) noexcept;

    private:
        math::color _PixelShader(const Ray& ray, const Scene& scene, size_t recursion_depth) const noexcept;

    private:
        void _TreadTileRenderFunc(
            uint32_t tile_left_up_x, uint32_t tile_left_up_y, 
            uint32_t tile_right_down_x, uint32_t tile_right_down_y, 
            const std::vector<gfx::Ray>& rays, 
            const gfx::Scene& scene
        ) noexcept;

        void _TreadTileRawAntialiasingFunc(
            uint32_t raw_left_up_x, uint32_t raw_left_up_y, 
            uint32_t raw_right_down_x, uint32_t raw_right_down_y
        ) noexcept;
    
    public:
    #pragma region getters-setters
        void SetAntialiasingLevel(AntialiasingLevel level) noexcept;
        AntialiasingLevel GetAntialiasingLevel() const noexcept;

        void SetOutputFrameSize(const math::vec2f& size) noexcept;
        const math::vec2f& GetOutputFrameSize() const noexcept;

        void SetBackgroundColor(const math::color& color) noexcept;
        const math::color& GetBackgroundColor() const noexcept;

        void SetReflectionDepth(size_t depth) noexcept;
        size_t GetReflectionDepth() const noexcept;
    #pragma endregion getters-setters

    private:
        std::vector<uint32_t> m_frame;
        math::vec2f m_frame_size;
        
        AntialiasingLevel m_antialiasing_level = AntialiasingLevel::NONE;
        size_t m_reflection_depth = 0;
        
        math::color m_background = math::color::BLACK;

        util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };
    };
}
#pragma once
#include "graphics/scene.hpp"
#include "objects/camera.hpp"

#include "thread_pool/thread_pool.hpp"

#include <vector>
        

namespace gfx {
    enum class AntialiasingLevel { NONE = 1, X2 = 2, X4 = 4, X8 = 8, X12 = 12, X16 = 16 };

    class Renderer {
    public:
        Renderer() = default;

        const std::vector<std::uint32_t>& Render(const Scene& scene, const Camera& camera) noexcept;

    private:
        Color _PixelShader(const Ray& ray, const Scene& scene, std::size_t recursion_depth) const noexcept;

    private:
        void _TreadTileRenderFunc(
            std::uint32_t tile_left_up_x, std::uint32_t tile_left_up_y, 
            std::uint32_t tile_right_down_x, std::uint32_t tile_right_down_y, 
            const std::vector<gfx::Ray>& rays, 
            const gfx::Scene& scene
        ) noexcept;

        void _TreadTileRawAntialiasingFunc(
            std::uint32_t raw_left_up_x, std::uint32_t raw_left_up_y, 
            std::uint32_t raw_right_down_x, std::uint32_t raw_right_down_y
        ) noexcept;
    
    public:
    #pragma region getters-setters
        void SetAntialiasingLevel(AntialiasingLevel level) noexcept;
        AntialiasingLevel GetAntialiasingLevel() const noexcept;

        void SetOutputFrameSize(const math::vec2ui& size) noexcept;
        const math::vec2ui& GetOutputFrameSize() const noexcept;

        void SetBackgroundColor(Color color) noexcept;
        Color GetBackgroundColor() const noexcept;

        void SetReflectionDepth(std::size_t depth) noexcept;
        std::size_t GetReflectionDepth() const noexcept;
    #pragma endregion getters-setters

    private:
        std::vector<std::uint32_t> m_frame;
        math::vec2ui m_frame_size;
        
        AntialiasingLevel m_antialiasing_level = AntialiasingLevel::NONE;
        std::size_t m_reflection_depth = 0;
        
        Color m_background = Color::BLACK;

        util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };
    };
}
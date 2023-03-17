#pragma once
#include "graphics/scene.hpp"

#include <vector>
        
// #define MT

namespace gfx {
    enum class AntialiasingLevel : std::size_t { NONE = 0, X2 = 2, X4 = 4, X8 = 8, X16 = 16, X32 = 32 };

    class Renderer {
    public:
        Renderer() = default;

        void SetAntialiasingLevel(AntialiasingLevel level) noexcept;
        AntialiasingLevel GetAntialiasingLevel() const noexcept;

        const std::vector<std::uint32_t>& Render(const gfx::Scene& scene, std::uint32_t width, std::uint32_t height, float fov, const gfx::Color& background) noexcept;

    private:
        std::vector<std::uint32_t> m_frame;
        AntialiasingLevel m_antialiasing_level = AntialiasingLevel::NONE;

        #ifdef MT
            std::vector<std::uint32_t> m_vertical_it;   
        #endif

    };
}
#pragma once
#include "graphics/scene.hpp"
#include "objects/camera.hpp"

#include <vector>
        

namespace gfx {
    enum class AntialiasingLevel : std::size_t { NONE = 0, X2 = 2, X4 = 4, X8 = 8, X16 = 16 };

    class Renderer {
    public:
        Renderer() = default;

        void SetAntialiasingLevel(AntialiasingLevel level) noexcept;
        AntialiasingLevel GetAntialiasingLevel() const noexcept;

        void SetOutputFrameSize(const math::vec2ui& size) noexcept;
        const math::vec2ui& GetOutputFrameSize() const noexcept;

        const std::vector<std::uint32_t>& Render(const gfx::Scene& scene, const Camera& camera, const gfx::Color& background) noexcept;

    private:
        std::vector<std::uint32_t> m_frame;
        math::vec2ui m_frame_size;
        
        AntialiasingLevel m_antialiasing_level = AntialiasingLevel::NONE;

        std::vector<std::uint32_t> m_vertical_it;  
    };
}
#pragma once
#include "graphics/scene.hpp"
#include "objects/camera.hpp"

#include <vector>
        

namespace gfx {
    enum class AntialiasingLevel { NONE = 1, X2 = 2, X4 = 4, X8 = 8 };

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
        math::vec2ui m_antialiasing_frame_size;
        
        AntialiasingLevel m_antialiasing_level = AntialiasingLevel::NONE;

        std::vector<std::uint32_t> m_vertical_it;
    };
}
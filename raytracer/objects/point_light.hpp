#pragma once
#include "light.hpp"

namespace gfx {
    class PointLigth : public ILight {
    public:
        PointLigth() = default;
        PointLigth(const math::vec3f& position, const gfx::Color& color, float intensity);
        ~PointLigth() override;

        bool ComputeIllumination(const math::vec3f& intersect_point, const math::vec3f& normal, 
            const std::list<std::shared_ptr<IDrawable>>& drawables, 
            const std::shared_ptr<IObject>& curr_obj, 
            gfx::Color& color, float& intensity) const noexcept override;
    };
}
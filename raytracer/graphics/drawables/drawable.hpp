#pragma once
#include <optional>

#include "shape.hpp"

#include "graphics/ray.hpp"
#include "graphics/intersection_data.hpp"

#include "math_3d/vec3.hpp"

#include <vector>
#include <memory>

namespace raytracing::gfx {
    class IDrawable : public IShape {
    public:
        IDrawable() = default;
        IDrawable(const math::vec3f& position, std::shared_ptr<IMaterial> material) 
            : IShape(), m_position(position), m_material(material) {}

        virtual std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept = 0;

        virtual void MoveFor(const math::vec3f& dist) noexcept { m_position += dist; }

    #pragma region vars
        math::vec3f m_position;
        std::shared_ptr<IMaterial> m_material;
    #pragma endregion vars
    };
}
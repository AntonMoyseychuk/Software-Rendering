#pragma once
#include <optional>

#include "shape.hpp"

#include "graphics/ray.hpp"
#include "graphics/intersection_data.hpp"

#include <vector>
#include <memory>

namespace gfx {
    class IDrawable : public IShape {
    public:
        IDrawable() = default;
        IDrawable(const math::vec3f& position, std::shared_ptr<IMaterial> material) 
            : IShape(position), m_material(material) {}

        virtual std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept = 0;

    #pragma region getters-setters
        void SetMaterial(std::shared_ptr<IMaterial> material) noexcept { m_material = material; }
        std::shared_ptr<IMaterial> GetMaterial() const noexcept { return m_material; }
    #pragma endregion getters-setters

    protected:
        std::shared_ptr<IMaterial> m_material;
    };
}
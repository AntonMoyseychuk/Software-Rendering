#pragma once
#include <optional>

#include "shape.hpp"

#include "graphics/ray.hpp"
#include "graphics/intersection_data.hpp"

#include <vector>

namespace gfx {
    class IDrawable : public IShape {
    public:
        IDrawable() = default;
        IDrawable(const math::vec3f& position, const Material& material) 
            : IShape(position), m_material(material) {}

        virtual std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept = 0;

    #pragma region getters-setters
        void SetMaterial(const Material& material) noexcept { m_material = material; }
        const Material& GetMaterial() const noexcept { return m_material; }
    #pragma endregion getters-setters

    protected:
        Material m_material;
    };
}
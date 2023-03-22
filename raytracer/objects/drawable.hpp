#pragma once
#include <optional>

#include "shape.hpp"

#include "graphics/ray.hpp"
#include "graphics/material.hpp"

#include <vector>

namespace gfx {
    struct IntersectionData {
        IntersectionData() = default;
        IntersectionData(const math::vec3f& _point, const math::vec3f& _normal, const Material& _material, float _distance) 
            : point(_point), normal(_normal), material(_material), distance(_distance) {}

        math::vec3f point;
        math::vec3f normal;
        Material material;

        float distance;
    };

    class IDrawable : public IShape {
    public:
        IDrawable() = default;
        IDrawable(const math::vec3f& position, const Material& material) 
            : IShape(position), m_material(material) {}

        virtual std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept = 0;

        void SetMaterial(const Material& material) noexcept { m_material = material; }
        const Material& GetMaterial() const noexcept { return m_material; }
        Material& GetMaterial() noexcept { return m_material; }

    protected:
        Material m_material;
    };
}
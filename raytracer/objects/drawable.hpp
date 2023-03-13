#pragma once
#include <optional>

#include "object.hpp"

#include "graphics/ray.hpp"
#include "graphics/material.hpp"

#include <vector>

namespace gfx {
    struct IntersectionData {
        IntersectionData() = default;
        IntersectionData(const math::vec3f& _point, const math::vec3f& _normal, const Color& _color) 
            : point(_point), normal(_normal), color(_color) {}

        math::vec3f point;
        math::vec3f normal;
        Color color;
    };

    class IDrawable : public IObject {
    public:
        IDrawable() = default;
        IDrawable(const math::vec3f& position, const Material& material) 
            : IObject(position), m_material(material) {}

        virtual std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept = 0;

        void SetMaterial(const Material& material) noexcept { m_material = material; }
        const Material& GetMaterial() const noexcept { return m_material; }
        Material& GetMaterial() noexcept { return m_material; }

    protected:
        Material m_material;
    };
}
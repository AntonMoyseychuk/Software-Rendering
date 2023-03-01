#pragma once
#include "math_3d/vector.hpp"
#include "graphics/ray.hpp"

#include "graphics/material.hpp"

namespace gfx {
    class Object {
    public:
        virtual ~Object() = default;
        Object() = default;
        Object(const math::vec4f& position, const Material& material) : m_position(position), m_material(material) {}
        
        virtual bool IsIntersect(const Ray& ray, math::vec4f& int_point, math::vec4f& int_point_normal, Color& int_point_color) const noexcept = 0;

        void SetPositon(const math::vec4f& position) noexcept { m_position = position; }
        const math::vec4f& GetPositon() const noexcept { return m_position; }
        math::vec4f& GetPositon() noexcept { return m_position; }

        void SetMaterial(const Material& material) noexcept { m_material = material; }
        const Material& GetMaterial() const noexcept { return m_material; }
        Material& GetMaterial() noexcept { return m_material; }

    protected:
        math::vec4f m_position;
        Material m_material;
    };
}
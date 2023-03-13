#pragma once
#include "object.hpp"

#include "graphics/ray.hpp"
#include "graphics/material.hpp"

#include <vector>

namespace gfx {
    class IDrawable : public IObject {
    public:
        IDrawable() = default;
        IDrawable(const math::vec3f& position, const Material& material) 
            : IObject(position), m_material(material) {}

        virtual bool IsIntersect(const Ray& ray, 
                                    math::vec3f& int_point, 
                                    math::vec3f& int_point_normal, 
                                    Color& int_point_color) const noexcept = 0;
        
        //                         std::uint32_t buffer_width, 
        //                         std::uint32_t buffer_height, 
        //                         const math::vec4f& camera_pos, 
        //                         const math::vec4f& camera_dir, 
        //                         float FOV) const noexcept = 0;

        void SetMaterial(const Material& material) noexcept { m_material = material; }
        const Material& GetMaterial() const noexcept { return m_material; }
        Material& GetMaterial() noexcept { return m_material; }

    protected:
        Material m_material;
    };
}
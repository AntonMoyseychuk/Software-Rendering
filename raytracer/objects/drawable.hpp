#pragma once
#include "object.hpp"

#include "graphics/ray.hpp"
#include "graphics/material.hpp"

#include <vector>

namespace gfx {
    class Drawable : public Object {
    public:
        Drawable() = default;
        Drawable(const math::vec4f& position, const Material& material) 
            : Object(position), m_material(material) {}

        virtual bool IsIntersect(const Ray& ray, 
                                    math::vec4f& int_point, 
                                    math::vec4f& int_point_normal, 
                                    Color& int_point_color) const noexcept = 0;
        
        // virtual void Render(std::vector<std::uint32_t>& buffer, 
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
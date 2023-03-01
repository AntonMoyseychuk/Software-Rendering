#pragma once
#include "object.hpp"

namespace gfx {
    class Sphere : public Object {
    public:
        Sphere() = default;
        Sphere(const math::vec4f& position, float radius, const Material& material);

        bool IsIntersect(const Ray& ray, math::vec4f& intersect_point, math::vec4f& local_normal, Color& local_color) const noexcept override;

        void SetRadius(float radius) noexcept;
        float GetRadius() const noexcept;
        float& GetRadius() noexcept;

    private:
        float m_radius = 0.0f;
    };
    
}
#include "sphere.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    Sphere::Sphere(const math::vec3f& position, float r, const gfx::Material &material)
        : IDrawable(position, material), m_radius(r)
    {
    }

    Sphere::~Sphere()
    {
    }

    std::optional<IntersectionData> Sphere::IsIntersect(const Ray& ray) const noexcept {
        using namespace math;

        vec3f k = ray.original - m_position;
        float b = 2.0f * Dot(k, ray.direction);
        float c = Dot(k,k) - m_radius * m_radius;
        float d = b * b - 4.0f * c;

        if (d > 0) {
            float sqrtd = sqrtf(d);

            float t1 = (-b + sqrtd) / 2.0f;
            float t2 = (-b - sqrtd) / 2.0f;

            float t = t2;

            auto intersect_point = ray.original + ray.direction * t;
            auto local_normal = (intersect_point - m_position).Normalize();
            auto local_color = m_material.color;
            
            return t > 0 ? IntersectionData{intersect_point, local_normal, local_color} : std::optional<IntersectionData>{};
        }

        return {};
    }

    void Sphere::SetRadius(float radius) noexcept {
        assert(radius >= 0.0f);
        m_radius = radius;
    }

    float Sphere::GetRadius() const noexcept {
        return m_radius;
    }

    float &Sphere::GetRadius() noexcept {
        return m_radius;
    }
}
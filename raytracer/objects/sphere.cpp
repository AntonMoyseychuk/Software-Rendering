#include "sphere.hpp"
#include "math_3d/math.hpp"

#include <algorithm>

#include <algorithm>

namespace gfx {
    Sphere::Sphere(const math::vec3f& p, float r, const gfx::Material &m)
        : IDrawable(p, m), m_radius(r)
    {
    }

    Sphere::~Sphere()
    {
    }

    bool Sphere::IsIntersect(const Ray& ray, math::vec3f& intersect_point, math::vec3f& local_normal, Color& local_color) const noexcept {
        using namespace math;

        vec3f k = ray.original - m_position;
        float b = Dot(k, ray.direction);
        float c = Dot(k,k) - m_radius * m_radius;
        float d = b * b - c;

        if (d > 0) {
            float sqrtd = sqrtf(d);

            float t1 = (-b + sqrtd) / 2.0f;
            float t2 = (-b - sqrtd) / 2.0f;

            float min_t = std::min(t1, t2);
            float max_t = std::max(t1, t2);

            float t = (min_t >= 0) ? min_t : max_t;
            intersect_point = ray.original + ray.direction * t;
            
            local_normal = (intersect_point - m_position).Normalize();
            local_color = m_material.color;
            
            return t > 0;
        }

        return false;
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
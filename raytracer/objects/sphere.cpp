#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

#include <algorithm>

namespace gfx {
    Sphere::Sphere(const math::vec4f& p, float r, const gfx::Material &m)
        : Drawable(p, m), m_radius(r)
    {
    }

    bool Sphere::IsIntersect(const Ray& ray, math::vec4f& intersect_point, math::vec4f& local_normal, Color& local_color) const noexcept {
        using namespace math;

        vec4f k = ray.original - m_position;
        float b = LinMath::Dot(k, ray.direction);
        float c = LinMath::Dot(k,k) - m_radius * m_radius;
        float d = b * b - c;

        if (d > 0) {
            float sqrtd = sqrtf(d);

            float t1 = (-b + sqrtd) / 2.0f;
            float t2 = (-b - sqrtd) / 2.0f;

            float min_t = std::min(t1, t2);
            float max_t = std::max(t1, t2);

            float t = (min_t >= 0) ? min_t : max_t;
            intersect_point = ray.original + ray.direction * t;
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
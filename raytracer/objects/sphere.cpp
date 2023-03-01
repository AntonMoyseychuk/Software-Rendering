#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

namespace gfx {
    Sphere::Sphere(const math::vec4f& p, float r, const gfx::Material &m)
        : Object(p, m), m_radius(r)
    {
    }

    bool Sphere::IsIntersect(const Ray& ray, math::vec4f& intersect_point, math::vec4f& local_normal, Color& local_color) const noexcept {
        using namespace math;

        vec4f AB = m_position - ray.original;

        auto AC = LinMath::Dot<float, float>(AB, ray.direction) / ray.direction.Length();
        if (LinMath::IsTendsTo(AC, decltype(AC)(0))) {
            return false;
        }

        auto square_d = LinMath::Dot<float>(AB, AB) - AC * AC;

        if (square_d <= m_radius * m_radius) {
            //
            local_color = m_material.color;
            //
            return true;
        } else {
            return false;
        }
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
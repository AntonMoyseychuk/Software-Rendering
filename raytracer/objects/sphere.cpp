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

        vec3f k = ray.origin - m_position;
        float b = 2.0f * Dot(k, ray.direction);
        float c = Dot(k,k) - m_radius * m_radius;
        float d = b * b - 4.0f * c;

        if (d >= 0.0f) {
            float half_sqrtd = sqrtf(d) / 2.0f;
            float half_b = b / 2.0f;

            // float t1 = (-b + sqrtd) / 2.0f;
            // float t2 = (-b - sqrtd) / 2.0f;

            float t = -half_b - half_sqrtd;

            auto intersect_point = ray.origin + ray.direction * t;
            auto local_normal = math::Normalize(intersect_point - m_position);
            
            return t > 0 ? IntersectionData(intersect_point, local_normal, m_material, t) : std::optional<IntersectionData>{};
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
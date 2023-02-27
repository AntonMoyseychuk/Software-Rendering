#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

namespace shape {
    Sphere::Sphere(const math::Vector3f& p, float r, const gfx::Material &m)
        : position(p), radius(r), material(m)
    {
    }

    bool Sphere::IsIntersect(const math::Ray &ray) const noexcept
    {
        using namespace math;

        Vector3f AB = position - ray.original;

        auto AC = LinMath::Dot(AB, ray.direction) / ray.direction.Length();
        if (AC < 0.000001f) {
            return false;
        }

        auto square_d = LinMath::Dot(AB, AB) - AC * AC;

        return square_d <= radius * radius;
    }
}
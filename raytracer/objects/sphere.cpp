#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

namespace gfx {
    Sphere::Sphere(const math::Vector<float>& p, float r, const gfx::Material &m)
        : position(p), radius(r), material(m)
    {
    }

    bool Sphere::IsIntersect(const Ray &ray) const noexcept
    {
        using namespace math;

        Vector<float> AB = position - ray.original;

        auto AC = LinMath::Dot<float, float>(AB, ray.direction) / ray.direction.Length();
        if (AC < 0.000001f) {
            return false;
        }

        auto square_d = LinMath::Dot<float>(AB, AB) - AC * AC;

        return square_d <= radius * radius;
    }
}
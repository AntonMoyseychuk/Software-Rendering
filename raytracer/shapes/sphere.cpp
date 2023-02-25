#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

namespace shape {
    Sphere::Sphere(const math::Vector3f &pos, float r)
        : position(pos), radius(r)
    {
    }
    
    bool Sphere::IsRayIntersect(const math::Vector3f &ray_orig, const math::Vector3f &ray_dir) const noexcept {
        using namespace math;

        Vector3f AB = position - ray_orig;

        auto AC = LinMath::Dot(AB, ray_dir) / ray_dir.Length();
        if (AC < 0.000001f) {
            return false;
        }

        auto square_d = LinMath::Dot(AB, AB) - AC * AC;

        return square_d <= radius * radius;
    }
}
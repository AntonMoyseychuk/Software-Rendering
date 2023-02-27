#include "math_3d/ray.hpp"

namespace math {
    Ray::Ray(const Vec3f &o, const Vec3f &dir) 
        : original(o), direction(dir) 
    {
    }
}
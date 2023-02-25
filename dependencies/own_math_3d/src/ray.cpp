#include "math_3d/ray.hpp"

namespace math {
    Ray::Ray(const Vector3f &o, const Vector3f &dir) 
        : original(o), direction(dir) 
    {
    }
}
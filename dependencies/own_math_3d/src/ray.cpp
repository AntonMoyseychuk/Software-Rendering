#include "math_3d/ray.hpp"

namespace math {
    Ray::Ray(const Vector<float> &o, const Vector<float> &dir) 
        : original(o), direction(dir) 
    {
    }
}
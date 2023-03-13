#include "ray.hpp"

namespace gfx {
    Ray::Ray(const math::vec3f& o, const math::vec3f& dir) 
        : original(o), direction(dir) 
    {
    }
}
#include "ray.hpp"

namespace gfx {
    Ray::Ray(const math::vec3f& o, const math::vec3f& dir) 
        : origin(o), direction(dir) 
    {
    }
}
#include "ray.hpp"

namespace gfx {
    Ray::Ray(const math::vec4f& o, const math::vec4f& dir) 
        : original(o), direction(dir) 
    {
    }
}
#include "ray.hpp"

namespace raytracing::gfx {
    Ray::Ray(const math::vec3& o, const math::vec3& dir) 
        : origin(o), direction(dir) 
    {
    }
}
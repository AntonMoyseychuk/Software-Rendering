#include "material.hpp"

namespace gfx {
    Material::Material(const Color &c, float specular, float reflective)
        : color(c), specular_index(specular), reflective_index(reflective)
    {
    }
}
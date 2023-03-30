#include "material.hpp"

namespace gfx {
    Material::Material(const Color &c, float specular, float reflective, float refractive)
        : color(c), specular_index(specular), reflective_index(reflective), refraction_index(refraction_index)
    {
    }
}
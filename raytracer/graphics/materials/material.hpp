#pragma once
#include "math_3d/vec4.hpp"
#include "graphics/ray.hpp"

namespace raytracing::gfx {
    struct IntersectionData;
    struct IMaterial {
        virtual bool Scatter(const IntersectionData& int_data, math::color& attenuation, Ray& scattered_ray) const noexcept = 0;
    };
}
#pragma once
#include "graphics/color.hpp"
#include "graphics/ray.hpp"

namespace raytracing::gfx {
    struct IntersectionData;
    struct IMaterial {
        virtual bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept = 0;
    };
}
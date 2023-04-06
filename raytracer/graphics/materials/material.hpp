#pragma once
#include "graphics/color.hpp"
#include "graphics/ray.hpp"

namespace gfx {
    struct IntersectionData;
    class IMaterial {
    public:
        virtual bool Scatter(const IntersectionData& int_data, Color& attenuation, Ray& scattered_ray) const noexcept = 0;
    };
}
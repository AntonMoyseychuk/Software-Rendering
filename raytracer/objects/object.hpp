#pragma once
#include "math_3d/vector.hpp"
#include "graphics/ray.hpp"

#include "graphics/color.hpp"

namespace gfx {
    class IObject {
    public:
        virtual ~IObject() = default;
        virtual bool IsIntersect(const Ray& ray, math::vec4f& int_point, math::vec4f& int_point_normal, math::Color& int_point_color) const noexcept = 0;
    };
}
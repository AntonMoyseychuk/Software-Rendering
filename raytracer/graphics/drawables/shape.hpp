#pragma once
#include "math_3d/vector.hpp"

namespace gfx {
    class IShape {
    public:
        IShape() = default;
        virtual ~IShape() = 0 {}
    };
}
#pragma once

namespace raytracing::gfx {
    class IShape {
    public:
        IShape() = default;
        virtual ~IShape() = 0 {}
    };
}
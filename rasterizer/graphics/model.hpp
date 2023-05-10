#pragma once
#include "math_3d/vector.hpp"
#include <vector>

namespace rasterization::gfx {
    struct Model {
        Model() = default;
        Model(const char* filepath);

        bool Load(const char* filepath) noexcept;

        std::vector<math::vec3f> vertexes;
        std::vector<size_t> indexes;
    };
}
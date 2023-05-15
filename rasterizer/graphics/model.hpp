#pragma once
#include "math_3d/vector.hpp"
#include <vector>

namespace rasterization::gfx {
    struct Model {
        Model() = default;
        Model(const char* filepath);

        bool Load(const char* filepath) noexcept;

        std::vector<math::vec3f> vertexes;
        std::vector<math::vec3f> normals;
        std::vector<math::vec2f> texture_coords;
        std::vector<math::vec3ul> indexes;
    };
}
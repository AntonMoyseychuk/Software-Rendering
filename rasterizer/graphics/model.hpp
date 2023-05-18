#pragma once
#include "math_3d/vec3.hpp"
#include "math_3d/vec2.hpp"
#include <vector>

namespace rasterization::gfx {
    
    struct Model {
        Model() = default;
        Model(const char* filepath);

        bool Load(const char* filepath) const noexcept;
        void Free() const noexcept;

        mutable std::vector<math::vec3> vertexes;
        mutable std::vector<math::vec3> normals;
        mutable std::vector<math::vec2> texture_coords;
        mutable std::vector<math::vec3> indexes;
    };
}
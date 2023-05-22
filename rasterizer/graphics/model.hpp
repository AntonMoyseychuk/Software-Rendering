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

        mutable std::vector<math::vec3f> vertexes;
        mutable std::vector<math::vec3f> normals;
        mutable std::vector<math::vec2f> texture_coords;
        mutable std::vector<size_t> vert_indexes;
        mutable std::vector<size_t> texture_indexes;
        mutable std::vector<size_t> normal_indexes;
    };
}
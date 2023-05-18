#pragma once
#include "../../own-math-3d/math_3d/vec3.hpp"
#include "../../own-math-3d/math_3d/vec2.hpp"
#include <vector>

namespace util {
    class OBJParser final {
    public:
        static bool Parse(const char* filepath, 
            std::vector<math::vec3>& vertexes, 
            std::vector<math::vec3>& normals,
            std::vector<math::vec2>& texture_coords, 
            std::vector<math::vec3>& indexes
        ) noexcept;
    };
}
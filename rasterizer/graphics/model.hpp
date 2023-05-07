#pragma once
#include "math_3d/vector.hpp"
#include <vector>

namespace rasterization::gfx {
    class Model {
    public:
        Model() = default;
        Model(const char* filepath);

        const std::vector<math::vec3f>& GetVertexes() const noexcept { return m_verts; };
        std::vector<math::vec3f>& GetVertexes() noexcept { return m_verts; };
        
        const std::vector<size_t>& GetIndexes() const noexcept { return m_indexes; };

    private:
        std::vector<math::vec3f> m_verts;
        std::vector<size_t> m_indexes;
    };
}
#include "model.hpp"

#include "obj/obj_parser.hpp"

#include <string>
#include <stdexcept>

namespace rasterization::gfx {
    Model::Model(const char *filepath) {
        if (!Load(filepath)) {
            throw std::invalid_argument(std::string("Invalid filepath: ") + filepath);
        }
    }

    void Model::Free() const noexcept {
        std::vector<math::vec3f>().swap(vertexes);
        std::vector<math::vec3f>().swap(normals);
        std::vector<math::vec2f>().swap(texture_coords);
        std::vector<size_t>().swap(vert_indexes);
        std::vector<size_t>().swap(texture_indexes);
        std::vector<size_t>().swap(normal_indexes);
    }

    bool Model::Load(const char *filepath) const noexcept {
        return util::OBJParser::Parse(filepath, vertexes, normals, texture_coords, vert_indexes, texture_indexes, normal_indexes);
    }
}
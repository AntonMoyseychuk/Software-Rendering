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
        vertexes.clear();
        normals.clear();
        texture_coords.clear();
        indexes.clear();
    }

    bool Model::Load(const char *filepath) const noexcept {
        return util::OBJParser::Parse(filepath, vertexes, normals, texture_coords, indexes);
    }
}
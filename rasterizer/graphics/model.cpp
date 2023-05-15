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
    
    bool Model::Load(const char *filepath) noexcept {
        return util::OBJParser::Parse(filepath, vertexes, normals, texture_coords, indexes);
    }
}
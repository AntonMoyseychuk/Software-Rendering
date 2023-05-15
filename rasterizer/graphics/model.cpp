#include "model.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

#include <iostream>

namespace rasterization::gfx {
    Model::Model(const char *filepath) {
        if (!Load(filepath)) {
            throw std::invalid_argument(std::string("Invalid filepath: ") + filepath);
        }
    }
    
    bool Model::Load(const char *filepath) noexcept {
        std::ifstream file(filepath);

        if (!file.is_open()) {
            return false;
        }

        std::string str;
        while (!file.eof()) {
            std::getline(file, str);

            if (str.find("#", 0) != std::string::npos) {
                continue;
            }
            
            std::stringstream ss(str);
            char key;
            if (!str.compare(0, 2, "v ")) {
                math::vec3f v;

                ss >> key >> v.x >> v.y >> v.z;
                vertexes.emplace_back(v);
            } else if (!str.compare(0, 3, "vn ")) {
                math::vec3f n;

                ss >> key >> key >> n.x >> n.y >> n.z;
                normals.emplace_back(n);
            } else if (!str.compare(0, 3, "vt ")) {
                math::vec2f uv;

                ss >> key >> key >> uv.x >> uv.y;
                texture_coords.emplace_back(uv);
            } else if (!str.compare(0, 2, "f ")) {
                math::vec3ul vertex_indexes;
                char slash;

                ss >> key;

                for (size_t i = 0; i < 3; ++i) {
                    ss >> vertex_indexes[0] >> slash >> vertex_indexes[1] >> slash >> vertex_indexes[2];
                    indexes.emplace_back(vertex_indexes - math::vec3ul(1));
                }
            }
        }

        return true;
    }
}
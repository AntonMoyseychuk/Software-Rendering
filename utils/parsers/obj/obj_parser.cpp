#include "obj_parser.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace util {
    bool OBJParser::Parse(const char *filepath, 
        std::vector<math::vec3f> &vertexes, 
        std::vector<math::vec3f> &normals, 
        std::vector<math::vec2f> &texture_coords, 
        std::vector<size_t> &vert_indexes, 
        std::vector<size_t> &texture_indexes,
        std::vector<size_t> &normal_indexes
    ) noexcept {
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
                size_t vert_idx, texture_idx, normal_idx;
                char slash;

                ss >> key;

                for (size_t i = 0; i < 3; ++i) {
                    ss >> vert_idx >> slash >> texture_idx >> slash >> normal_idx;
                    vert_indexes.emplace_back(vert_idx - 1);
                    texture_indexes.emplace_back(texture_idx - 1);
                    normal_indexes.emplace_back(normal_idx - 1);
                }
            }
        }

        return true;
    }
}
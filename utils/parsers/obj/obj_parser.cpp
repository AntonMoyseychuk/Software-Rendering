#include "obj_parser.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace util {
    bool OBJParser::Parse(const char* filepath, 
        std::vector<math::vec3f>& vertexes, 
        std::vector<math::vec3f>& normals, 
        std::vector<math::vec2f>& texture_coords, 
        std::vector<math::vec3f>& indexes
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
                math::vec3f vertex_indexes;
                char slash;

                ss >> key;

                for (size_t i = 0; i < 3; ++i) {
                    ss >> vertex_indexes[0] >> slash >> vertex_indexes[1] >> slash >> vertex_indexes[2];
                    indexes.emplace_back(vertex_indexes - math::vec3f(1));
                }
            }
        }

        return true;
    }
}
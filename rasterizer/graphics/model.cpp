#include "model.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

#include <iostream>

namespace rasterization::gfx {
    Model::Model(const char *filepath) {
        std::ifstream file(filepath);

        if (!file.is_open()) {
            throw std::invalid_argument(std::string("Invalid file path: ") + filepath);
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
                m_verts.emplace_back(v);
            } else if (!str.compare(0, 2, "f ")) {
                size_t index;
                char slash;

                ss >> key;

                for (size_t i = 0; i < 3; ++i) {
                    ss >> index;
                    m_indexes.emplace_back(index - 1);
                    ss >> slash >> index >> slash >> index;
                }
            }
        }
    }
}
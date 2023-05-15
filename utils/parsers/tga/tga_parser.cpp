#include "tga_parser.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace util {
    bool TGAParser::Parse(const char* filepath, 
        uint32_t& width, 
        uint32_t& height,
        uint32_t& chanel_count,
        std::vector<math::Color>& colors
    ) noexcept {
        std::ifstream file(filepath, std::ios_base::binary);

        if (!file.is_open()) {
            return false;
        }

        uint8_t header[18];
        file.read((char*)header, 18);

        // If not RGB image, return false
        if(header[2] != 2) { 
            return false;
        }

        width = header[13] * 256 + header[12];
        height = header[15] * 256 + header[14];

        chanel_count = header[16] / 8;

        const size_t size = width * height * chanel_count;
        std::vector<uint8_t> image(size);
        file.read((char*)&image[0], size);

        // TGA holds BGR data, but we want it to be RGB instead. So we need to swap channels.
        colors.resize(width * height);
        for(size_t i = 0; i < size; i += chanel_count) {
            colors[i / chanel_count] = math::Color(image[i + 2], image[i + 1], image[i], 255);
        }

        return true;
    }
}
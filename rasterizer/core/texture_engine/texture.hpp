#pragma once
#include <vector>
#include <cstdint>

namespace gl {
    struct _texture {
        _texture() = default;
        _texture(uint32_t width, uint32_t height, uint8_t channel_count, const void* data);

        std::vector<uint8_t> data;
        uint32_t width;
        uint32_t height;
        uint8_t channel_count;
    };
}
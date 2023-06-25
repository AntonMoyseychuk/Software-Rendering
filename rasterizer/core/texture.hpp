#pragma once
#include <cstdint>
#include <vector>

namespace gl {
    enum class texture_format : uint8_t {
        RGB = 3, RGBA = 4
    };

    struct _texture final {
        _texture() = default;
        _texture(size_t width, size_t height, texture_format format, const void* data);

        std::vector<uint8_t> data;
        size_t width;
        size_t height;
        texture_format format;
    };
}
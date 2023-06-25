#include "texture.hpp"

namespace gl {
    _texture::_texture(size_t width, size_t height, texture_format format, const void *data)
        : data((const uint8_t*)data, (const uint8_t*)data + width * height * static_cast<size_t>(format)), width(width), height(height), format(format)
    {
    }
}
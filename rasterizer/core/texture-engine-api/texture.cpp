#include "texture.hpp"

namespace gl {
    _texture::_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void* data)
        : data((const uint8_t*)data, (const uint8_t*)data + width * height * channel_count), width(width), height(height), channel_count(channel_count)
    {
    }
}
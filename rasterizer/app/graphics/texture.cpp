#include "texture.hpp"

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <stdexcept>

namespace rasterization {
    std::unordered_map<std::string, Texture::Content> Texture::already_loaded_textures;

    Texture::Texture(const char *filename) {
        if (!Load(filename)) {
            throw std::runtime_error(stbi_failure_reason());
        }
    }
    
    const Texture::Content* Texture::Load(const char *filename) noexcept {
        if (already_loaded_textures.count(filename) == 1) {
            return &already_loaded_textures[filename];
        }

        Content content;
        uint8_t* data = stbi_load(filename, &content.width, &content.height, &content.channel_count, 0);
        if (data == nullptr) {
            return nullptr;
        }

        content.data = std::vector<uint8_t>(data, data + content.width * content.height * content.channel_count);
        stbi_image_free(data);

        already_loaded_textures[filename] = content;
        m_content = &already_loaded_textures[filename];
        return m_content;
    }
    
    const Texture::Content *Texture::GetContent() const noexcept {
        return m_content;
    }
}
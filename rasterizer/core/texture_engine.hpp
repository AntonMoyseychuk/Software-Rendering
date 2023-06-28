#pragma once
#include <unordered_map>

#include "texture.hpp"

#define ASSERT_TEXTURE_ID_VALIDITY(container, texture_id) ASSERT(container.find((texture_id)) != container.cend(), "texture engine error", "invalid texture ID")

namespace gl {
    struct _texture_engine {
        static _texture_engine& get() noexcept;

        size_t create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void* data) noexcept;
        void bind_texture(size_t id) noexcept;

    private:
        _texture_engine() = default;

        using texture_id = size_t;

    public:
        std::unordered_map<texture_id, _texture> textures;
        texture_id binded_texture = 0;
    };
}
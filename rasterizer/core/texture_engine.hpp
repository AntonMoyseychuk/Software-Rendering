#pragma once
#include <unordered_map>

#include "texture.hpp"
#include "assert_macro.hpp"

#define ASSERT_TEXTURE_ID_VALIDITY(container, texture_id) ASSERT(container.find((texture_id)) != container.cend(), "texture engine error", "invalid texture ID")

namespace gl {
    struct _texture_engine final {
        _texture_engine(const _texture_engine& engine) = delete;
        _texture_engine& operator=(const _texture_engine& engine) = delete;

        static _texture_engine& get() noexcept;

        size_t create_texture(size_t width, size_t height, texture_format format, const void* data) noexcept;
        void bind_texture(size_t id) noexcept;

    private:
        _texture_engine() = default;

    public:
        using texture_id = size_t;
        
        std::unordered_map<texture_id, _texture> textures;
        size_t binded_texture = 0;
    };
    
}
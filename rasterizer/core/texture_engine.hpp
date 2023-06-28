#pragma once
#include "texture.hpp"

#include <unordered_map>


namespace gl {
    struct _texture_engine {
        using texture_id = size_t;
        
        static _texture_engine& get() noexcept;

        size_t create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void* data) noexcept;
        void bind_texture(size_t id) noexcept;

    public:
        const _texture& _get_binded_texture() const noexcept;

    private:
        _texture_engine() = default;

    private:
        std::unordered_map<texture_id, _texture> m_textures;
        texture_id m_binded_texture = 0;
    };
}
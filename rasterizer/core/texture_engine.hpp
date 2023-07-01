#pragma once
#include <unordered_map>
#include "texture.hpp"

namespace gl {
    class _texture_engine final {
    public:
        static _texture_engine& get() noexcept;

        size_t create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void* data) noexcept;
        void bind_texture(size_t id) noexcept;

    public:
        const _texture& _get_binded_texture() const noexcept;

    private:
        _texture_engine() = default;

    private:
        std::unordered_map<size_t, _texture> m_textures;
        size_t m_binded_texture = 0;
    };
}
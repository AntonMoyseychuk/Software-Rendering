#pragma once
#include "texture_engine.hpp"

namespace gl {
    class _texture_engine_api {
    public:
        _texture_engine_api();

        size_t create_texture(uint32_t width, uint32_t height, uint8_t channel_count, const void* data) const noexcept;
        void bind_texture(size_t id) const noexcept;

    private:
        _texture_engine& m_tex_engine;
    };
}
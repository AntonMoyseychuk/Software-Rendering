#pragma once
#include <vector>
#include <string>
#include <unordered_map>


namespace rasterization {
    class Texture {
    public:
        struct Content {
            std::vector<uint8_t> data;
            int32_t width;
            int32_t height;
            int32_t channel_count;
        };

    public:
        Texture() noexcept = default;
        Texture(const char* filename);

        const Content* Load(const char* filename) noexcept;

        const Content* GetContent() const noexcept;

    private:
        static std::unordered_map<std::string, Content> already_loaded_textures;

    private:
        Content* m_content = nullptr;
    };
}
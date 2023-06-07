#pragma once
#include <unordered_map>
#include <vector>

namespace rasterization::gfx {
    enum class buffer_type : uint8_t { 
        VERTEX, INDEX
    };

    struct _buffer_engine final {
        _buffer_engine(const _buffer_engine& engine) = delete;
        _buffer_engine& operator=(const _buffer_engine& engine) = delete;

        static _buffer_engine& get() noexcept;

        size_t create_vertex_buffer(const void* buffer, size_t size) noexcept;
        size_t create_index_buffer(const size_t* buffer, size_t count) noexcept;
        
        void erase_buffer(size_t id) noexcept;
        void erase_index_buffer(size_t id) noexcept;

        void bind(buffer_type type, size_t id) noexcept;

    private:   
        _buffer_engine() = default;

    public:
        struct vertex_buffer {
            std::vector<uint8_t> data;
        };

        struct index_buffer {
            std::vector<size_t> data;
        };

        std::unordered_map<size_t, vertex_buffer> vbos;
        std::unordered_map<size_t, index_buffer> ibos;

        size_t curr_vbo = 0;
        size_t curr_ibo = 0;
    };
}
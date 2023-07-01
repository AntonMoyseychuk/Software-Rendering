#pragma once
#include <unordered_map>
#include <vector>

namespace gl {
    enum class buffer_type : uint8_t { 
        VERTEX, INDEX
    };

    class _buffer_engine final {
    public:
        _buffer_engine(const _buffer_engine& engine) = delete;
        _buffer_engine& operator=(const _buffer_engine& engine) = delete;

        static _buffer_engine& get() noexcept;

        size_t create_vertex_buffer(const void* buffer, size_t size) noexcept;
        size_t create_index_buffer(const size_t* buffer, size_t count) noexcept;
        
        void delete_vertex_buffer(size_t id) noexcept;
        void delete_index_buffer(size_t id) noexcept;

        void bind_buffer(buffer_type type, size_t id) noexcept;

        void set_buffer_element_size(size_t size) noexcept;

    private:   
        _buffer_engine() = default;

    public:
        struct vertex_buffer {
            std::vector<uint8_t> data;
            size_t element_size;
        };
        const vertex_buffer& _get_binded_vertex_buffer() const noexcept;
        
        struct index_buffer {
            std::vector<size_t> data;
        };
        const index_buffer& _get_binded_index_buffer() const noexcept;

        using buffer_id = size_t;

    private:
        std::unordered_map<buffer_id, vertex_buffer> m_vbos;
        std::unordered_map<buffer_id, index_buffer> m_ibos;

        buffer_id m_binded_vbo = 0;
        buffer_id m_binded_ibo = 0;
    };
}
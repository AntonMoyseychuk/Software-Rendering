#pragma once
#include <unordered_map>
#include <vector>

namespace rasterization::gfx {
    enum class attrib_data_type : uint8_t { 
        INT8 = 1, 
        UINT8, 
        INT16, 
        UINT16, 
        INT32, 
        UINT32, 
        INT64, 
        UINT64, 
        FLOAT, 
        DOUBLE
    };

    enum class buffer_type : uint8_t { 
        VERTEX, INDEX
    };

    struct _buffer_engine final {
        static _buffer_engine& get() noexcept;

        size_t create_vertex_buffer(const void* buffer, size_t size) noexcept;
        void vertex_attrib_pointer(size_t vbo_id, size_t size, attrib_data_type type, size_t stride, const void* ptr) noexcept;

        size_t create_index_buffer(const size_t* buffer, size_t count) noexcept;
        
        void erase_buffer(size_t id) noexcept;
        void erase_index_buffer(size_t id) noexcept;

        void bind(buffer_type type, size_t id) noexcept;

    private:   
        _buffer_engine() = default;

    public:
        struct attribute {
            attribute() noexcept = default;
            attribute(size_t size, attrib_data_type type, size_t stride, const void* ptr) noexcept
                : size(size), type(type), stride(stride), ptr(ptr) {}

            size_t size;
            attrib_data_type type;
            size_t stride;
            const void* ptr;
        };

        struct buffer {
            std::vector<uint8_t> data;
            std::vector<attribute> attributes;
        };

        std::unordered_map<size_t, buffer> m_vbos;
        std::unordered_map<size_t, std::vector<size_t>> m_ibos;

        size_t m_curr_vbo = 0;
        size_t m_curr_ibo = 0;
    };
}
#pragma once
#include "buffer_engine.hpp"

namespace gl {
    class _buffer_engine_api {
    public:
        _buffer_engine_api();

        size_t create_vertex_buffer(const void* buffer, size_t size) const noexcept;
        size_t create_index_buffer(const size_t* buffer, size_t count) const noexcept;
        
        void delete_vertex_buffer(size_t id) const noexcept;
        void delete_index_buffer(size_t id) const noexcept;

        void bind_buffer(buffer_type type, size_t id) const noexcept;

        void set_buffer_element_size(size_t size) const noexcept;
    
    private:
        _buffer_engine& m_buffer_engine;
    };
}
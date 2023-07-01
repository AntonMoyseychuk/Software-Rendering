#include "buffer_engine_api.hpp"

namespace gl {
    _buffer_engine_api::_buffer_engine_api()
        : m_buffer_engine(_buffer_engine::get())
    {
    }

    size_t _buffer_engine_api::create_vertex_buffer(const void *buffer, size_t size) const noexcept {
        return m_buffer_engine.create_vertex_buffer(buffer, size);
    }

    size_t _buffer_engine_api::create_index_buffer(const size_t *buffer, size_t count) const noexcept {
        return m_buffer_engine.create_index_buffer(buffer, count);
    }

    void _buffer_engine_api::delete_vertex_buffer(size_t id) const noexcept {
        m_buffer_engine.delete_vertex_buffer(id);
    }

    void _buffer_engine_api::delete_index_buffer(size_t id) const noexcept {
        m_buffer_engine.delete_index_buffer(id);
    }

    void _buffer_engine_api::bind_buffer(buffer_type type, size_t id) const noexcept {
        m_buffer_engine.bind_buffer(type, id);
    }

    void _buffer_engine_api::set_buffer_element_size(size_t size) const noexcept {
        m_buffer_engine.set_buffer_element_size(size);
    }
}
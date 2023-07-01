#include "buffer_engine.hpp"
#include "math_3d/util.hpp"

#include "core/assert_macro.hpp"

#define _ASSERT_BUFFER_ID_VALIDITY(container, id) ASSERT(container.find((id)) != container.cend(), "buffer engine error", "invalid buffer ID")

namespace gl {
    _buffer_engine &_buffer_engine::get() noexcept {
        static _buffer_engine _buffer_engine;
        return _buffer_engine;
    }
    
    size_t _buffer_engine::create_vertex_buffer(const void *buffer, size_t size) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_vbos.find(id) != m_vbos.cend());

        m_vbos[id] = vertex_buffer { std::vector<uint8_t>((uint8_t*)buffer, (uint8_t*)buffer + size), 0 };
    
        return id;
    }

    size_t _buffer_engine::create_index_buffer(const size_t *buffer, size_t count) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_ibos.find(id) != m_ibos.cend());

        m_ibos[id] = index_buffer {
            std::vector<size_t>(buffer, buffer + count)
        };

        return id;
    }

    void _buffer_engine::delete_vertex_buffer(size_t id) noexcept {
        m_vbos.erase(id);
    }

    void _buffer_engine::delete_index_buffer(size_t id) noexcept {
        m_ibos.erase(id);
    }
    
    void _buffer_engine::bind_buffer(buffer_type type, size_t id) noexcept {
        switch (type) {
        case buffer_type::VERTEX:
            _ASSERT_BUFFER_ID_VALIDITY(m_vbos, id);
            m_binded_vbo = id;
            break;
        
        case buffer_type::INDEX:
            _ASSERT_BUFFER_ID_VALIDITY(m_ibos, id);
            m_binded_ibo = id;
            break;

        default:
            ASSERT(false, "buffer engine error", "invalid buffer_type");
            break;
        }
    }
    
    void _buffer_engine::set_buffer_element_size(size_t size) noexcept {
        _ASSERT_BUFFER_ID_VALIDITY(m_vbos, m_binded_vbo);
        m_vbos[m_binded_vbo].element_size = size;
    }
    
    const _buffer_engine::vertex_buffer &_buffer_engine::_get_binded_vertex_buffer() const noexcept {
        _ASSERT_BUFFER_ID_VALIDITY(m_vbos, m_binded_vbo);
        return m_vbos.at(m_binded_vbo);
    }
    
    const _buffer_engine::index_buffer &_buffer_engine::_get_binded_index_buffer() const noexcept {
        _ASSERT_BUFFER_ID_VALIDITY(m_ibos, m_binded_ibo);
        return m_ibos.at(m_binded_ibo);
    }
}
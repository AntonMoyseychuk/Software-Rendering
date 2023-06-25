#include "buffer_engine.hpp"
#include "math_3d/util.hpp"

namespace gl {
    _buffer_engine &_buffer_engine::get() noexcept {
        static _buffer_engine _buffer_engine;
        return _buffer_engine;
    }
    
    size_t _buffer_engine::create_vertex_buffer(const void *buffer, size_t size) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (vbos.count(id) != 0);

        vbos[id] = vertex_buffer { std::vector<uint8_t>((uint8_t*)buffer, (uint8_t*)buffer + size), 0 };
    
        return id;
    }

    size_t _buffer_engine::create_index_buffer(const size_t *buffer, size_t count) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (ibos.count(id) != 0);

        ibos[id] = index_buffer {
            std::vector<size_t>(buffer, buffer + count)
        };

        return id;
    }

    void _buffer_engine::delete_vertex_buffer(size_t id) noexcept {
        vbos.erase(id);
    }

    void _buffer_engine::delete_index_buffer(size_t id) noexcept {
        ibos.erase(id);
    }
    
    void _buffer_engine::bind_buffer(buffer_type type, size_t id) noexcept {
        switch (type) {
        case buffer_type::VERTEX:
            ASSERT_BUFFER_ID_VALIDITY(vbos, id);
            binded_vbo = id;
            break;
        
        case buffer_type::INDEX:
            ASSERT_BUFFER_ID_VALIDITY(ibos, id);
            binded_ibo = id;
            break;

        default:
            ASSERT(false, "buffer engine error", "invalid buffer_type");
            break;
        }
    }
    
    void _buffer_engine::set_buffer_element_size(size_t size) noexcept {
        ASSERT_BUFFER_ID_VALIDITY(vbos, binded_vbo);
        vbos[binded_vbo].element_size = size;
    }
    
    const _buffer_engine::vertex_buffer &_buffer_engine::_get_binded_vertex_buffer() const noexcept {
        ASSERT_BUFFER_ID_VALIDITY(vbos, binded_vbo);
        return vbos.at(binded_vbo);
    }
    
    const _buffer_engine::index_buffer &_buffer_engine::_get_binded_index_buffer() const noexcept {
        ASSERT_BUFFER_ID_VALIDITY(ibos, binded_ibo);
        return ibos.at(binded_ibo);
    }
}
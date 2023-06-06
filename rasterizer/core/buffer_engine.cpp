#include "buffer_engine.hpp"
#include "math_3d/util.hpp"

#include <cassert>

namespace rasterization::gfx {
    _buffer_engine &_buffer_engine::get() noexcept {
        static _buffer_engine _buffer_engine;
        return _buffer_engine;
    }
    
    size_t _buffer_engine::create_vertex_buffer(const void *buffer, size_t size) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (vbos.count(id) != 0);

        vbos[id] = _buffer_engine::buffer {
            std::vector<uint8_t>((uint8_t*)buffer, (uint8_t*)buffer + size), 
            std::vector<attribute>() 
        };
    
        return id;
    }

    void _buffer_engine::vertex_attrib_pointer(size_t vbo_id, size_t size, attrib_data_type type, size_t stride, const void *ptr) noexcept {
        assert(vbos.count(vbo_id) > 0);
        vbos[vbo_id].attributes.emplace_back(size, type, stride, ptr);
    }

    size_t _buffer_engine::create_index_buffer(const size_t *buffer, size_t count) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (ibos.count(id) != 0);

        ibos[id] = std::vector<size_t>(buffer, buffer + count);

        return id;
    }

    void _buffer_engine::erase_buffer(size_t id) noexcept {
        vbos.erase(id);
    }

    void _buffer_engine::erase_index_buffer(size_t id) noexcept {
        ibos.erase(id);
    }
    
    void _buffer_engine::bind(buffer_type type, size_t id) noexcept {
        switch (type) {
        case buffer_type::VERTEX:
            assert(vbos.count(id) > 0);
            curr_vbo = id;
            break;
        
        case buffer_type::INDEX:
            assert(ibos.count(id) > 0);
            curr_ibo = id;
            break;

        default:
            assert(false && "invalid buffer_type");
            break;
        }
    }
}
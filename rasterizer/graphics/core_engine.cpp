#include "core_engine.hpp"
#include "math_3d/util.hpp"

#include <cassert>

namespace rasterization::gfx {
    const CoreEngine &CoreEngine::Get() noexcept {
        static CoreEngine engine;
        return engine;
    }
    
    size_t CoreEngine::CreateBuffer(BufferType type, const void* buffer, size_t size) const noexcept {
        switch (type) {
        case BufferType::VERTEX:
            return _CreateVertexBuffer(buffer, size);

        default:
            assert(false && "Invalid Buffer Type");
        };

        return 0;
    }

    void CoreEngine::VertexAttribPointer(size_t vbo_id, size_t index, size_t size, 
        AttribDataType type, size_t stride, const void *pointer
    ) const noexcept {
        if (m_vbos.count(vbo_id) != 0) {
            m_vbos[vbo_id].attributes[index] = { size, type, stride, pointer };
        }
    }

    size_t CoreEngine::CreateIndexBuffer(const size_t *buffer, size_t count) const noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_ibos.count(id) != 0);

        m_ibos[id] = std::vector<size_t>(buffer, buffer + count);

        return id;
    }

    void CoreEngine::EraseBuffer(size_t id) const noexcept {
        m_vbos.erase(id);
    }

    void CoreEngine::EraseIndexBuffer(size_t id) const noexcept {
        m_ibos.erase(id);
    }

    void CoreEngine::SetShaderUniform(const std::string& uniform_name, const math::mat4f &mat) const noexcept {
        m_mat4_uniforms[uniform_name] = mat;
    }

    size_t CoreEngine::_CreateVertexBuffer(const void* buffer, size_t size) const noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_vbos.count(id) != 0);

        m_vbos[id] = _Buffer {
            std::vector<uint8_t>((uint8_t*)buffer, (uint8_t*)buffer + size), 
            std::unordered_map<size_t, _Attribute>() 
        };
    
        return id;
    }
}
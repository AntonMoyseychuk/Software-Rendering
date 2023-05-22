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
            return _CreateVertexBuffer((const math::vec3f*)buffer, size / sizeof(math::vec3f));

        default:
            assert(false && "Invalid Buffer Type");
        };

        return 0;
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

    size_t CoreEngine::_CreateVertexBuffer(const math::vec3f *buffer, size_t count) const noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_vbos.count(id) != 0);

        m_vbos[id] = std::vector<math::vec3f>(buffer, buffer + count);
    
        return id;
    }
}
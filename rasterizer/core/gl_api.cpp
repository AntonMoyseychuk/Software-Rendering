#include "gl_api.hpp"
#include "math_3d/util.hpp"

#include <cassert>

namespace rasterization::gfx {
    const GLApi &GLApi::Get() noexcept {
        static GLApi engine;
        return engine;
    }
    
    size_t GLApi::CreateBuffer(BufferType type, const void* buffer, size_t size) const noexcept {
        switch (type) {
        case BufferType::VERTEX:
            return _CreateVertexBuffer(buffer, size);

        default:
            assert(false && "Invalid Buffer Type");
        };

        return 0;
    }

    void GLApi::VertexAttribPointer(size_t vbo_id, size_t index, size_t size, 
        AttribDataType type, size_t stride, const void *pointer
    ) const noexcept {
        if (m_vbos.count(vbo_id) != 0) {
            m_vbos[vbo_id].attributes[index] = { size, type, stride, pointer };
        }
    }

    size_t GLApi::CreateIndexBuffer(const size_t *buffer, size_t count) const noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (m_ibos.count(id) != 0);

        m_ibos[id] = std::vector<size_t>(buffer, buffer + count);

        return id;
    }

    void GLApi::EraseBuffer(size_t id) const noexcept {
        m_vbos.erase(id);
    }

    void GLApi::EraseIndexBuffer(size_t id) const noexcept {
        m_ibos.erase(id);
    }

    void GLApi::SetShaderUniform(const std::string& uniform_name, const math::mat4f &mat) const noexcept {
        m_mat4_uniforms[uniform_name] = mat;
    }

    void GLApi::SetShaderUniform(const std::string &uniform_name, const math::vec4f &vec) const noexcept {
        m_vec4f_uniforms[uniform_name] = vec;
    }

    void GLApi::SetShaderUniform(const std::string &uniform_name, const math::vec3f &vec) const noexcept {
        m_vec3f_uniforms[uniform_name] = vec;
    }

    void GLApi::SetShaderUniform(const std::string &uniform_name, const math::vec2f &vec) const noexcept {
        m_vec2f_uniforms[uniform_name] = vec;
    }

    void GLApi::SetShaderUniform(const std::string &uniform_name, float value) const noexcept {
        m_float_uniforms[uniform_name] = value;
    }

    size_t GLApi::_CreateVertexBuffer(const void* buffer, size_t size) const noexcept {
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
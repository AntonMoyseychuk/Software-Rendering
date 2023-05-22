#pragma once
#include "math_3d/vec4.hpp"
#include "math_3d/mat4.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace rasterization::gfx {
    enum class BufferType : uint8_t { VERTEX = 1 };
    enum class AttribDataType : uint8_t { INT8 = 1, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64, FLOAT, DOUBLE };

    class CoreEngine final {
    private:
        struct _Attribute {
            size_t size;
            AttribDataType type;
            size_t stride;
            const void* ptr;
        };

        struct _Buffer {
            std::vector<uint8_t> data;
            std::unordered_map<size_t, _Attribute> attributes;
        };

    public:
        friend class Rasterizer;

        static const CoreEngine& Get() noexcept;

        size_t CreateBuffer(BufferType type, const void* buffer, size_t size) const noexcept;
        void VertexAttribPointer(size_t vbo_id, size_t index, size_t size, AttribDataType type, size_t stride, const void* pointer) const noexcept;

        size_t CreateIndexBuffer(const size_t* buffer, size_t count) const noexcept;
        
        void EraseBuffer(size_t id) const noexcept;
        void EraseIndexBuffer(size_t id) const noexcept;

        void SetShaderUniform(const std::string& uniform_name, const math::mat4f& mat) const noexcept;

    private:
        CoreEngine() = default;

    private:
        size_t _CreateVertexBuffer(const void* buffer, size_t size) const noexcept;

    private:
        mutable std::unordered_map<size_t, _Buffer> m_vbos;
        mutable std::unordered_map<size_t, std::vector<size_t>> m_ibos;

        mutable std::unordered_map<std::string, math::mat4f> m_mat4_uniforms;
    };
}
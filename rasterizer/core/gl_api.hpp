#pragma once
#include "math_3d/vec4.hpp"
#include "math_3d/mat4.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace rasterization::gfx {
    enum class BufferType : uint8_t { VERTEX = 1 };
    enum class AttribDataType : uint8_t { INT8 = 1, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64, FLOAT, DOUBLE };

    class GLApi final {
    private:
        struct _Attribute {
            _Attribute() noexcept = default;
            _Attribute(size_t size, AttribDataType type, size_t stride, const void* ptr) noexcept
                : size(size), type(type), stride(stride), ptr(ptr) {}

            size_t size;
            AttribDataType type;
            size_t stride;
            const void* ptr;
        };

        struct _Buffer {
            std::vector<uint8_t> data;
            std::vector<_Attribute> attributes;
        };

    public:
        friend class Rasterizer;

        static const GLApi& Get() noexcept;

        size_t CreateBuffer(BufferType type, const void* buffer, size_t size) const noexcept;
        void VertexAttribPointer(size_t vbo_id, size_t size, AttribDataType type, size_t stride, const void* ptr) const noexcept;

        size_t CreateIndexBuffer(const size_t* buffer, size_t count) const noexcept;
        
        void EraseBuffer(size_t id) const noexcept;
        void EraseIndexBuffer(size_t id) const noexcept;

        void SetShaderUniform(const std::string& uniform_name, const math::mat4f& mat) const noexcept;
        void SetShaderUniform(const std::string& uniform_name, const math::vec4f& vec) const noexcept;
        void SetShaderUniform(const std::string& uniform_name, const math::vec3f& vec) const noexcept;
        void SetShaderUniform(const std::string& uniform_name, const math::vec2f& vec) const noexcept;
        void SetShaderUniform(const std::string& uniform_name, float value) const noexcept;

    private:
        GLApi() = default;

    private:
        size_t _CreateVertexBuffer(const void* buffer, size_t size) const noexcept;

    private:
        mutable std::unordered_map<size_t, _Buffer> m_vbos;
        mutable std::unordered_map<size_t, std::vector<size_t>> m_ibos;

        mutable std::unordered_map<std::string, math::mat4f> m_mat4_uniforms;
        mutable std::unordered_map<std::string, math::vec4f> m_vec4f_uniforms;
        mutable std::unordered_map<std::string, math::vec3f> m_vec3f_uniforms;
        mutable std::unordered_map<std::string, math::vec2f> m_vec2f_uniforms;

        mutable std::unordered_map<std::string, float> m_float_uniforms;
    };
}
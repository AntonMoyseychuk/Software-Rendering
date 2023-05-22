#pragma once
#include "math_3d/vec4.hpp"
#include "math_3d/mat4.hpp"

#include <unordered_map>
#include <vector>

namespace rasterization::gfx {
    enum class BufferType : uint8_t { VERTEX };

    class CoreEngine final {
    public:
        friend class Rasterizer;

        static const CoreEngine& Get() noexcept;

        size_t CreateBuffer(BufferType type, const void* buffer, size_t size) const noexcept;
        // std::vector<math::vec3f>& GetVertexBuffer(size_t id) noexcept;
        size_t CreateIndexBuffer(const size_t* buffer, size_t count) const noexcept;
        
        void EraseBuffer(size_t id) const noexcept;
        void EraseIndexBuffer(size_t id) const noexcept;

    private:
        CoreEngine() = default;

    private:
        size_t _CreateVertexBuffer(const math::vec3f* buffer, size_t count) const noexcept;

    private:
        mutable std::unordered_map<size_t, std::vector<math::vec3f>> m_vbos;
        mutable std::unordered_map<size_t, std::vector<size_t>> m_ibos;
    };
}
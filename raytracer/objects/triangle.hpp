#pragma once
#include "drawable.hpp"
#include "graphics/vertex.hpp"


namespace gfx {
    class Triangle : public IDrawable {
    public:
        Triangle() = default;
        Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

        std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept override;
        void MoveFor(const math::vec3f& dist) noexcept override;

        const Vertex& operator[](const std::size_t index) const noexcept;
        Vertex& operator[](const std::size_t index) noexcept;

    private:
        Vertex m_v0 = { math::VECTOR_LEFT };
        Vertex m_v1 = { math::VECTOR_UP };
        Vertex m_v2 = { math::VECTOR_RIGHT };
    };
}
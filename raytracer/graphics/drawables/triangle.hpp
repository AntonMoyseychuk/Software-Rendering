#pragma once
#include "drawable.hpp"
#include "graphics/vertex.hpp"


namespace raytracing::gfx {
    class Triangle : public IDrawable {
    public:
        Triangle() = default;
        Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::shared_ptr<IMaterial> material = nullptr);

        std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept override;
        void MoveFor(const math::vec3& dist) noexcept override;

        const Vertex& operator[](const size_t index) const noexcept;
        Vertex& operator[](const size_t index) noexcept;

    private:
        Vertex m_v0 = { math::vec3::LEFT };
        Vertex m_v1 = { math::vec3::UP };
        Vertex m_v2 = { math::vec3::RIGHT };
    };
}
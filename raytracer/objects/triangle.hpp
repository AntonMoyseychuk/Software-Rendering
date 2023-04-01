#pragma once
#include "drawable.hpp"

namespace gfx {
    class Triangle : public IDrawable {
    public:
        Triangle() = default;
        Triangle(const math::vec3f& v0, const math::vec3f& v1, const math::vec3f& v2, const Material& material);

        std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept override;
        void MoveFor(const math::vec3f& dist) noexcept override;

        const math::vec3f& operator[](const std::size_t index) const noexcept;
        math::vec3f& operator[](const std::size_t index) noexcept;

    private:
        math::vec3f m_v0 = math::VECTOR_LEFT;
        math::vec3f m_v1 = math::VECTOR_UP;
        math::vec3f m_v2 = math::VECTOR_RIGHT;
    };
}
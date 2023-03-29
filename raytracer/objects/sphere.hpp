#pragma once
#include "drawable.hpp"

namespace gfx {
    class Sphere : public IDrawable {
    public:
        Sphere() = default;
        Sphere(const math::vec3f& position, float radius, const Material& material);
        ~Sphere() override;

        std::optional<IntersectionData> IsIntersect(const Ray& ray) const noexcept override;

    #pragma region getters-setters
        void SetRadius(float radius) noexcept;
        float GetRadius() const noexcept;
    #pragma endregion getters-setters

    private:
        float m_radius = 0.0f;
    };
    
}
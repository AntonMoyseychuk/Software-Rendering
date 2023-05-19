#pragma once
#include "drawable.hpp"

namespace raytracing::gfx {
    class Sphere : public IDrawable {
    public:
        Sphere() = default;
        Sphere(const math::vec3& position, float radius, std::shared_ptr<IMaterial> material);
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
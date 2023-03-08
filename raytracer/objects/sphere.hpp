#pragma once
#include "drawable.hpp"

namespace gfx {
    class Sphere : public IDrawable {
    public:
        Sphere() = default;
        Sphere(const math::vec4f& position, float radius, const Material& material);
        ~Sphere() override {}

        bool IsIntersect(const Ray& ray, math::vec4f& intersect_point, math::vec4f& local_normal, Color& local_color) const noexcept override;
        // void Render(std::vector<std::uint32_t>& buffer,std::uint32_t buffer_width, std::uint32_t buffer_height, 
        //     const math::vec4f& camera_pos, const math::vec4f& camera_dir, float FOV) const noexcept override;

        void SetRadius(float radius) noexcept;
        float GetRadius() const noexcept;
        float& GetRadius() noexcept;

    private:
        float m_radius = 0.0f;
    };
    
}
#pragma once
#include "graphics/ray.hpp"
#include "math_3d/math.hpp"

namespace gfx {
    class Camera {
    public:
        Camera(const math::vec3f& position, const math::vec3f& look_at, const math::vec3f& up, float fov_degrees, float aspect_ratio);

        const std::vector<gfx::Ray>& GenerateRays(const math::vec2ui& screen_size) const noexcept;
        
    #pragma region getters-setters
        float GetFOVInDegrees() const noexcept;
        float GetFOVInRadians() const noexcept;

        void SetAspectRatio(float aspect) noexcept;
        float GetAspectRatio() const noexcept;

        const math::vec2ui& GetRayCacheSize() const noexcept;

        math::vec3f& GetPosition() noexcept;
        const math::vec3f& GetPosition() const noexcept;
    #pragma endregion getters-setters

    private:
        math::mat4f m_view;

        float m_fov_degrees = 0.0f;
        float m_aspect_ratio = 0.0f;

        //
        math::vec3f m_dir;
        math::vec3f m_position;
        //

        mutable std::vector<gfx::Ray> m_ray_cache;
        mutable math::vec2ui m_ray_cache_size;    
    };


    // class Camera : protected IShape {
    // public:
    //     Camera() = default;
    //     Camera(const math::vec3f& position, const math::vec3f& look_at, const math::vec3f& up, float fov_degrees, float aspect_ratio)
    //         : IShape(position), m_fov(fov_degrees), m_aspect_ratio(aspect_ratio)
    //     {
    //         m_dir = (look_at - m_position).Normalize();
    //         const auto right = math::Cross(math::VECTOR_UP, -m_dir);
    //         const auto up = math::Cross(-m_dir, right);

    //         m_view = math::LookAtRH(m_position, look_at, up);
    //     }
        
    //     void Rotate(const math::vec3f& axis, float angle_degrees) noexcept {
    //         m_view = math::Rotate(m_view, axis, math::ToRadians(angle_degrees));
    //         m_dir = math::vec3f(math::vec4f(m_dir) * m_view);
    //     }
        
    //     void MoveFor(const math::vec3f& dist) noexcept {
    //         m_position += dist; 
    //         m_view = math::Translate(m_view, dist);
    //     }
        
    //     void SetPositon(const math::vec3f& position) noexcept { 
    //         m_view = math::Translate(m_view, position - m_position);
    //         m_position = position; 
    //     }

    //     const math::mat4f& GetView() const noexcept {
    //         return m_view;
    //     }

    //     const std::vector<gfx::Ray>& GenerateRays(const math::vec2ui& screen_size) const noexcept {
    //         if (screen_size.x != m_ray_cache_size.x && screen_size.y != m_ray_cache_size.y) {
    //             m_ray_cache.resize(screen_size.x * screen_size.y);
    //             m_ray_cache_size = screen_size;

    //             const float fov = tanf(math::ToRadians(m_fov) / 2.0f);
    //             const auto dx = 2.0f / screen_size.x;
    //             const auto dy = 2.0f / screen_size.y;

    //             for (std::size_t y = 0; y < screen_size.y; ++y) {
    //                 for (std::size_t x = 0; x < screen_size.x; ++x) {
    //                     float pixel_x = (-1.0f + (x * dx)) * m_aspect_ratio * fov;
    //                     float pixel_y = (1.0f - (y * dy)) * fov;

    //                     m_ray_cache[x + y * screen_size.x] = Ray(m_position, math::vec3f(pixel_x, pixel_y, m_dir.z).Normalize());
    //                 }
    //             }
    //         }

    //         return m_ray_cache;
    //     }
        
    // // private:
    //     math::mat4f m_view;
    //     math::vec3f m_dir;
        
    //     float m_fov = 0.0f;
    //     float m_aspect_ratio = 0.0f;

    //     mutable std::vector<gfx::Ray> m_ray_cache;
    //     mutable math::vec2ui m_ray_cache_size;    
    // };
}
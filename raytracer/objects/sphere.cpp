#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

#include <algorithm>

namespace gfx {
    Sphere::Sphere(const math::vec4f& p, float r, const gfx::Material &m)
        : Drawable(p, m), m_radius(r)
    {
    }

    bool Sphere::IsIntersect(const Ray& ray, math::vec4f& intersect_point, math::vec4f& local_normal, Color& local_color) const noexcept {
        using namespace math;

        vec4f k = ray.original - m_position;
        float b = LinMath::Dot(k, ray.direction);
        float c = LinMath::Dot(k,k) - m_radius * m_radius;
        float d = b * b - c;

        if (d > 0) {
            float sqrtd = sqrtf(d);

            float t1 = (-b + sqrtd) / 2.0f;
            float t2 = (-b - sqrtd) / 2.0f;

            float min_t = std::min(t1, t2);
            float max_t = std::max(t1, t2);

            float t = (min_t >= 0) ? min_t : max_t;
            intersect_point = ray.original + ray.direction * t;
            return t > 0;
        }

        return false;
    }

    // void Sphere::Render(std::vector<std::uint32_t> &buf, std::uint32_t w, std::uint32_t h, const math::vec4f &cam_pos, const math::vec4f &cam_dir, float FOV) const noexcept {
    //     assert(buf.size() == w * h);
    //     gfx::Ray ray(cam_pos, cam_dir);

    //     math::vec4f int_point, int_normal;
        
    //     for (std::size_t y = 0; y < h; ++y) {
    //         for (std::size_t x = 0; x < w; ++x) {
    //             float pixel_x = (2 * (x + 0.5f) / static_cast<float>(w) - 1) * FOV * w / static_cast<float>(h);
    //             float pixel_y = -(2 * (y + 0.5f) / static_cast<float>(h) - 1) * FOV;
    //             ray.direction = math::vec4f(pixel_x, pixel_y, ray.direction.z).Normalize();

    //             gfx::Color out_color(buf[x + y * w]);
                
    //             auto nearest_point = m_position - ray.original;
    //             nearest_point -= nearest_point.Normalize() * m_radius;

    //             if (this->IsIntersect(ray, int_point, int_normal, out_color)) {
    //                 // auto int_dist = (int_point - ray.original).Length();
                    
    //                 auto c = math::LinMath::Dot(int_point, nearest_point) / (int_point.Length() * nearest_point.Length());
    //                 c *= 10000;
    //                 c = static_cast<std::int32_t>(c) % 500;
    //                 c /= 500;
    //                 out_color = m_material.color * c;
    //             }

    //             buf[x + y * w] = out_color.rgba;
    //         }
    //     }
    // }

    void Sphere::SetRadius(float radius) noexcept {
        assert(radius >= 0.0f);

        m_radius = radius;
    }

    float Sphere::GetRadius() const noexcept {
        return m_radius;
    }

    float &Sphere::GetRadius() noexcept {
        return m_radius;
    }
}
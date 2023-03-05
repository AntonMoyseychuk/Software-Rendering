#include "sphere.hpp"
#include "math_3d/lin_math.hpp"

namespace gfx {
    Sphere::Sphere(const math::vec4f& p, float r, const gfx::Material &m)
        : Drawable(p, m), m_radius(r)
    {
    }

    // Sphere::~Sphere()
    // {
    // }

    bool Sphere::IsIntersect(const Ray& ray, math::vec4f& intersect_point, math::vec4f& local_normal, Color& local_color) const noexcept {
        using namespace math;

        // vec4f AB = m_position - ray.original;
        
        // auto AC = LinMath::Dot(AB, ray.direction) / ray.direction.Length();
        // if (LinMath::IsTendsTo(AC, 0.0f)) {
        //     return false;
        // }
        
        // auto square_d = LinMath::Dot(AB, AB) - AC * AC;
        
        // if (square_d <= m_radius * m_radius) {
        //     //
        //     local_color = m_material.color;
        //     //
        //     return true;
        // } else {
        //     return false;
        // }

        //dot(x,x) + dot(y, y) = r*r
        //y = x + dir * t
        //
        //dot(x,x) + dot(x + dir * t, x + dir * t) - r*r = 0
        //t*t + 2*x*t + 2*x*x-r*r = 0
        //d = 4*x*x - 8*x*x + 4r*r = 4(r*r - x*x)
        //t1,t2 = (-2x +- sqrt(d)) / 2.0f
        //

        auto b = 2.0f * LinMath::Dot(ray.original, ray.direction);
        auto c = LinMath::Dot(ray.original, ray.original) - m_radius * m_radius;
        auto d = b * b - 4.0f * c;
        
        if (d < 0) {
            return false;
        }
        
        if (LinMath::IsTendsTo(d, 0.0f)) {
            intersect_point = ray.original + ray.direction * (-b / 2.0f);
        } else {
            auto t1 = (-b + sqrtf(d)) / 2.0f;
            auto t2 = (-b - sqrtf(d)) / 2.0f;
         
            if (t1 < 0 || t2 < 0) {
                return false;
            }
        
            intersect_point = ray.original + ray.direction * (t1 < t2 ? t1 : t2);
        }
        
        return true;
    }

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
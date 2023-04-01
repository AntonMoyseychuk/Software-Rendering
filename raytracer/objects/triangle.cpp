#include "triangle.hpp"

namespace gfx {
    Triangle::Triangle(const math::vec3f &v0, const math::vec3f &v1, const math::vec3f &v2, const Material &material)
        : IDrawable(math::vec3f(0.0f), material), m_v0(v0), m_v1(v1), m_v2(v2)
    {
        const auto e = v1 - v0, mid_e = v0 + math::Normalize(e) * (e.Length() / 2.0f);
        const auto mediana = mid_e - v2, center = v2 + math::Normalize(mediana) * (mediana.Length() * 2.0f / 3.0f);
        m_position = center;
    }
    
    std::optional<IntersectionData> Triangle::IsIntersect(const Ray &ray) const noexcept {
        const auto e1 = m_v1 - m_v0;
        const auto e2 = m_v2 - m_v0;

        const auto pvec = math::Cross(ray.direction, e2);
        const auto det = math::Dot(e1, pvec);

        if (math::IsTendsTo(det, 0.0f)) {
            return {};
        }

        const auto inv_det = 1.0f / det;
        const auto tvec = ray.origin - m_v0;

        const auto u = math::Dot(tvec, pvec) * inv_det;
        if (u < 0.0f || u > 1.0f) {
            return {};
        }

        const auto qvec = math::Cross(tvec, e1);
        const auto v = math::Dot(ray.direction, qvec) * inv_det;
        if (v < 0.0f || u + v > 1.0f) {
            return {};
        }

        const auto t = math::Dot(e2, qvec) * inv_det;
        return IntersectionData(ray.origin + ray.direction * t, math::Normalize(math::Cross(e2, e1)), ray, t, m_material);
    }
    
    void Triangle::MoveFor(const math::vec3f &dist) noexcept {
        m_v0 += dist;
        m_v1 += dist;
        m_v2 += dist;
    }
    
    const math::vec3f& Triangle::operator[](const std::size_t index) const noexcept {
        assert(index < 3 && "index must be 0 <= index < 3");
        return *(&m_v0 + index);
    }
    
    math::vec3f& Triangle::operator[](const std::size_t index) noexcept {
        assert(index < 3 && "index must be 0 <= index < 3");
        return *(&m_v0 + index);
    }
}
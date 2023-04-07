#include "triangle.hpp"

namespace gfx {
    Triangle::Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::shared_ptr<IMaterial> material)
        : IDrawable(math::vec3f(), material), m_v0(v0), m_v1(v1), m_v2(v2)
    {
        const auto e = v1.position - v0.position, mid_e = v0.position + math::Normalize(e) * (e.Length() / 2.0f);
        const auto mediana = mid_e - v2.position, center = v2.position + math::Normalize(mediana) * (mediana.Length() * 2.0f / 3.0f);
        m_position = center;
    }

    std::optional<IntersectionData> Triangle::IsIntersect(const Ray &ray) const noexcept {
        // const auto e1 = m_v1 - m_v0;
        // const auto e2 = m_v2 - m_v0;
        // 
        // const auto pvec = math::Cross(ray.direction, e2);
        // const auto det = math::Dot(e1, pvec);
        // 
        // if (math::IsTendsTo(det, 0.0f)) {
        //     return {};
        // }
        // 
        // const auto inv_det = 1.0f / det;
        // const auto tvec = ray.origin - m_v0;
        // 
        // const auto u = math::Dot(tvec, pvec) * inv_det;
        // if (u < 0.0f || u > 1.0f) {
        //     return {};
        // }
        //
        // const auto qvec = math::Cross(tvec, e1);
        // const auto v = math::Dot(ray.direction, qvec) * inv_det;
        // if (v < 0.0f || u + v > 1.0f) {
        //     return {};
        // }
        //
        // const auto t = math::Dot(e2, qvec) * inv_det;
        // return IntersectionData(ray.origin + ray.direction * t, math::Normalize(math::Cross(e2, e1)), ray, t, m_material);

        using namespace math;

        const auto e0 = m_v2.position - m_v0.position;
        const auto e1 = m_v1.position - m_v2.position;

        const auto normal_not_normalized = Cross(e0, m_v1.position - m_v0.position);
        const auto normal = Normalize(normal_not_normalized);
        const auto dot_raydir_normal = Dot(ray.direction, normal);
        if (IsTendsTo(dot_raydir_normal, 0.0f)) { //parallel
            return {};
        }

        #if 1
            if (dot_raydir_normal >= 0.0f) { //backface culling
                return {};
            }
        #endif

        // Ax + Bx + Cx + D = 0
        // ray = O + R * t
        const auto D = -(normal.x * m_v0.position.x + normal.y * m_v0.position.y + normal.z * m_v0.position.z);
        const auto t = -(Dot(normal, ray.origin) + D) / dot_raydir_normal;
        if (t < 0.0f) {
            return {};
        }

        const auto hit_point = ray.origin + ray.direction * t;

        const auto v0_hit = hit_point - m_v0.position;
        auto C = Cross(e0, v0_hit);
        if (Dot(normal, Normalize(C)) <= 0.0f) {
            return {};
        }
        const auto u = C.Length() / normal_not_normalized.Length();

        const auto v2_hit = hit_point - m_v2.position;
        C = Cross(e1, v2_hit);
        if (Dot(normal, Normalize(C)) <= 0.0f) {
            return {};
        }
        const auto v = C.Length() / normal_not_normalized.Length();

        auto w = 1.0f - u - v;
        if (w < 0.0f) {
            return {};
        }

        // Material result_material(m_v0.material->color * u + m_v1.material->color * v + m_v2.material->color * w,
        //     m_v0.material->specular_index * u + m_v1.material->specular_index * v + m_v2.material->specular_index * w,
        //     m_v0.material->reflective_index * u + m_v1.material->reflective_index * v + m_v2.material->reflective_index * w
        // );
        // return IntersectionData(hit_point, normal, t, ray, result_material);

        // auto result_color = *m_v0.color * u + *m_v1.color * v + *m_v2.color * w;
        return IntersectionData(hit_point, normal, t, ray, m_material);
    }

    void Triangle::MoveFor(const math::vec3f &dist) noexcept {
        m_v0.position += dist;
        m_v1.position += dist;
        m_v2.position += dist;
    }
    
    const Vertex& Triangle::operator[](const std::size_t index) const noexcept {
        assert(index < 3 && "index must be 0 <= index < 3");
        return *(&m_v0 + index);
    }
    
    Vertex& Triangle::operator[](const std::size_t index) noexcept {
        assert(index < 3 && "index must be 0 <= index < 3");
        return *(&m_v0 + index);
    }
}
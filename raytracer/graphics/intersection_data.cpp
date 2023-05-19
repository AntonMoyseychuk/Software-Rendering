#include "intersection_data.hpp"
#include "math_3d/vec_operations.hpp"

namespace raytracing::gfx {
    IntersectionData::IntersectionData(
        const math::vec3& _point, 
        const math::vec3& _normal, 
        float _distance, 
        const Ray& _casted_ray, 
        std::shared_ptr<IMaterial> _material) : point(_point), normal(_normal), distance(_distance), casted_ray(_casted_ray), material(_material)
    {
        SetFaceNormal(_casted_ray, _normal);
    }
    
    void IntersectionData::SetFaceNormal(const Ray& ray, const math::vec3& outward_normal) noexcept {
        is_front_face = math::dot(ray.direction, outward_normal) < 0.0f;
        normal = is_front_face ? outward_normal : -outward_normal;
    }
}
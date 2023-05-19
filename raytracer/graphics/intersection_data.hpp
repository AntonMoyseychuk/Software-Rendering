#pragma once
#include "ray.hpp"
#include "materials/material.hpp"

#include <memory>

namespace raytracing::gfx {
    struct IntersectionData {
        IntersectionData() = default;
        IntersectionData(const math::vec3& point, const math::vec3& normal, float distance, const Ray& casted_ray, std::shared_ptr<IMaterial> material);

        void SetFaceNormal(const Ray& ray, const math::vec3& outward_normal) noexcept;

        math::vec3 point;
        math::vec3 normal;
        float distance;
        bool is_front_face;

        gfx::Ray casted_ray;
        std::shared_ptr<IMaterial> material;
    };
}
#pragma once
#include "ray.hpp"
#include "materials/material.hpp"

#include <memory>

namespace gfx {
    struct IntersectionData {
        IntersectionData() = default;
        IntersectionData(const math::vec3f& point, const math::vec3f& normal, float distance, const Ray& casted_ray, std::shared_ptr<IMaterial> material);

        void SetFaceNormal(const Ray& ray, const math::vec3f& outward_normal) noexcept;

        math::vec3f point;
        math::vec3f normal;
        float distance;
        bool is_front_face;

        gfx::Ray casted_ray;
        std::shared_ptr<IMaterial> material;
    };
}
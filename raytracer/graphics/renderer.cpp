#include "renderer.hpp"
#include "color.hpp"

#include "math_3d/math.hpp"

#include <algorithm>
#include <execution>


static math::vec3f _RandomVectorInUnitSphere() noexcept {
    while (true) {
        auto p = math::RandomVector<float, 3>(-1.0f,1.0f);
        if (p.Length() >= 1.0f) continue;
        return p;
    }
}

namespace gfx {
    void Renderer::SetAntialiasingLevel(AntialiasingLevel level) noexcept {
        m_antialiasing_level = level;
    }

    AntialiasingLevel Renderer::GetAntialiasingLevel() const noexcept {
        return m_antialiasing_level;
    }

    void Renderer::SetOutputFrameSize(const math::vec2ui &size) noexcept {
        if (m_frame_size.x != size.x || m_frame_size.y != size.y) {
            m_frame_size = size;
            
            m_frame.resize(m_frame_size.x * m_frame_size.y);

            m_vertical_it.resize(m_frame_size.y);
            for (std::size_t i = 0; i < m_frame_size.y; ++i) {
                m_vertical_it[i] = i;
            }
        }
    }

    const math::vec2ui &Renderer::GetOutputFrameSize() const noexcept {
        return m_frame_size;
    }

    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera, const gfx::Color& background) noexcept {
        const auto& rays = camera.GenerateRays(m_frame_size);
        
        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), 
            [this, &scene, &camera, &background, rays](std::uint32_t y) {
                const auto screen_width = m_frame_size.x;
                const auto screen_height = m_frame_size.y;

                for (std::size_t x = 0; x < screen_width; ++x) {
                    math::vec4f out_color(0.0f);

                    {
                        auto min_dist = math::MATH_INFINITY;
                        std::optional<gfx::IntersectionData> closest_intersection = {}, curr_intersection = {};
                        
                        for (const auto& drawable : scene.GetDrawables()) {
                            if ((curr_intersection = drawable->IsIntersect(rays[x + y * screen_width])).has_value()) {
                                auto int_point_dist = curr_intersection->distance;
                                    
                                if (int_point_dist < min_dist) {
                                    closest_intersection = curr_intersection;
                                    out_color = gfx::StoreColor<float>(closest_intersection->material.color);

                                    float sum_intensity = 0.0f;
                                    gfx::Color light_color;
                                    for (const auto& light : scene.GetLights()) {
                                        bool valid_illum = light->ComputeIllumination(closest_intersection->point, 
                                            closest_intersection->normal, light_color, sum_intensity);
                                    }

                                    out_color *= sum_intensity;
                                    min_dist = int_point_dist;
                                }
                            }
                        }

                        if (closest_intersection.has_value() == false) {
                            out_color = gfx::StoreColor<float>(background);
                        }
                    }

                    {
                        for (std::int32_t s = 0; s < static_cast<std::int32_t>(m_antialiasing_level) - 1; ++s) {
                            auto min_dist = math::MATH_INFINITY;
                            std::optional<gfx::IntersectionData> closest_intersection = {}, curr_intersection = {};

                            float sum_intensity = 0.0f;
                            for (const auto& drawable : scene.GetDrawables()) {
                                const auto dx = math::Random(-1, 1);
                                const auto dy = math::Random(-1, 1);

                                const auto index = math::Clamp(x + dx + (y + dy) * screen_width, 
                                    decltype(screen_width * screen_height)(0), screen_width * screen_height - 1);
                                
                                if ((curr_intersection = drawable->IsIntersect(rays[index])).has_value()) {

                                    auto int_point_dist = curr_intersection->distance;
                                        
                                    if (int_point_dist < min_dist) {
                                        closest_intersection = curr_intersection;
                                        
                                        sum_intensity = 0.0f;
                                        gfx::Color light_color;
                                        for (const auto& light : scene.GetLights()) {
                                            bool valid_illum = light->ComputeIllumination(closest_intersection->point, 
                                                closest_intersection->normal, light_color, sum_intensity);
                                        }

                                        min_dist = int_point_dist;
                                    }
                                }
                            }
                            
                            out_color += closest_intersection.has_value() ? 
                                gfx::StoreColor<float>(closest_intersection->material.color) * sum_intensity : gfx::StoreColor<float>(background);
                        }

                        if (m_antialiasing_level != AntialiasingLevel::NONE) {
                            out_color /= static_cast<float>(m_antialiasing_level);
                        }
                    }
                    
                    m_frame[x + y * screen_width] = gfx::LoadColor(out_color).rgba;
                }
            }
        );

        return m_frame;
    }
}
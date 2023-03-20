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
                    math::vec4f out_color = gfx::ToVector4<float>(background);

                    bool hit_anything = false;
                    auto min_dist = math::MATH_INFINITY;
                    for (const auto& drawable : scene.GetDrawables()) {
                        auto intersection = drawable->IsIntersect(rays[x + y * screen_width]);
                        if (intersection.has_value()) {
                            hit_anything = true;

                            auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                                
                            if (int_point_dist < min_dist) {
                                out_color = gfx::ToVector4<float>(intersection->material.color);

                                float intensity = 1.0f;
                                gfx::Color light_color;
                                for (const auto& light : scene.GetLights()) {
                                    bool valid_illum = light->ComputeIllumination(intersection->point, 
                                        intersection->normal, scene.GetDrawables(), light, light_color, intensity);
                                }

                                out_color *= intensity;
                                min_dist = int_point_dist;
                            }
                        }
                    }

                    for (std::int32_t s = 0; s < static_cast<std::int32_t>(m_antialiasing_level) - 1; ++s) {
                        bool hit_anything = false;
                        auto min_dist = math::MATH_INFINITY;
                        for (const auto& drawable : scene.GetDrawables()) {
                            const auto dx = math::Random(-1, 1);
                            const auto dy = math::Random(-1, 1);
                            const auto index = (x + (x > math::Abs(dx) && x < screen_width - math::Abs(dx) ? dx : 0)) 
                                + (y + (y > math::Abs(dy) && y < screen_height - math::Abs(dy) ? dy : 0)) * screen_width;
                            
                            auto intersection = drawable->IsIntersect(rays[index]);
                            if (intersection.has_value()) {
                                hit_anything = true;

                                auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                                    
                                if (int_point_dist < min_dist) {
                                    float intensity = 1.0f;
                                    gfx::Color light_color;
                                    for (const auto& light : scene.GetLights()) {
                                        bool valid_illum = light->ComputeIllumination(intersection->point, 
                                            intersection->normal, scene.GetDrawables(), light, light_color, intensity);
                                    }

                                    //out_color = intersection->color;
                                    out_color += gfx::ToVector4<float>(intersection->material.color) * intensity;
                                    min_dist = int_point_dist;
                                }
                            }
                        }

                        if (!hit_anything) {
                            out_color += gfx::ToVector4<float>(background);
                        }
                    }

                    if (m_antialiasing_level != AntialiasingLevel::NONE) {
                        out_color /= static_cast<float>(m_antialiasing_level);
                    }

                    m_frame[x + y * screen_width] = gfx::LoadColorFromVector(out_color).rgba;
                }
            }
        );

        return m_frame;
    }
}
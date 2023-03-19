#include "renderer.hpp"
#include "color.hpp"

#include "math_3d/math.hpp"

#include <algorithm>
#include <execution>

namespace gfx {
    void Renderer::SetAntialiasingLevel(AntialiasingLevel level) noexcept {
        m_antialiasing_level = level;
    }

    AntialiasingLevel Renderer::GetAntialiasingLevel() const noexcept {
        return m_antialiasing_level;
    }

    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera, const math::vec2ui& screen_size, const gfx::Color& background) noexcept {
        if (m_frame_size.x != screen_size.x && m_frame_size.y != screen_size.y) {
            m_frame.resize(screen_size.x * screen_size.y);

            m_vertical_it.resize(screen_size.y);
            for (std::size_t i = 0; i < screen_size.y; ++i) {
                m_vertical_it[i] = i;
            }
        }

        const auto& rays = camera.GenerateRays(screen_size);
        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), 
            [this, &scene, &camera, &background, rays](std::uint32_t y) {
                const auto screen_width = camera.GetRayCacheSize().x;

                for (std::size_t x = 0; x < screen_width; ++x) {
                    // gfx::Color out_color = background;
                    math::vec4f out_color = gfx::ToVector4<float>(background);

                    bool hit_anything = false;
                    auto min_dist = math::MATH_INFINITY;
                    for (const auto& drawable : scene.GetDrawables()) {
                        auto intersection = drawable->IsIntersect(rays[x + y * screen_width]);
                        if (intersection.has_value()) {
                            hit_anything = true;

                            //out_color = intersection->color;
                            out_color = gfx::ToVector4<float>(intersection->color);
                            auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                                
                            if (int_point_dist < min_dist) {
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

                    // for (std::size_t s = 0; s < static_cast<std::size_t>(m_antialiasing_level); ++s) {
                    //     pixel_x = (-1.0f + ((x + math::RandomInt64(-1, 1)) * dx)) * aspect * fov;
                    //     pixel_y = (1.0f - ((y + math::RandomInt64(-1, 1)) * dy)) * fov;
                    //     ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();

                    //     bool hit_anything = false;
                    //     auto min_dist = math::MATH_INFINITY;
                    //     for (const auto& drawable : scene.GetDrawables()) {
                    //         auto intersection = drawable->IsIntersect(ray);
                    //         if (intersection.has_value()) {
                    //             hit_anything = true;

                    //             auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                                    
                    //             if (int_point_dist < min_dist) {
                    //                 //out_color = intersection->color;
                    //                 out_color += gfx::ToVector4<float>(intersection->color);
                                    
                    //                 float intensity = 1.0f;
                    //                 gfx::Color light_color;
                    //                 for (const auto& light : scene.GetLights()) {
                    //                     bool valid_illum = light->ComputeIllumination(intersection->point, 
                    //                         intersection->normal, scene.GetDrawables(), light, light_color, intensity);
                    //                 }

                    //                 out_color *= intensity;
                    //                 min_dist = int_point_dist;
                    //             }
                    //         }
                    //     }

                    //     if (!hit_anything) {
                    //         out_color += gfx::ToVector4<float>(background);
                    //     }
                    // }

                    // if (m_antialiasing_level != AntialiasingLevel::NONE) {
                    //     out_color /= static_cast<float>(m_antialiasing_level);
                    // }

                    // m_frame[x + y * camera.GetViewportWidth()] = out_color.rgba;
                    m_frame[x + y * screen_width] = gfx::LoadColorFromVector(out_color).rgba;
                }
            }
        );

        return m_frame;
    }
}
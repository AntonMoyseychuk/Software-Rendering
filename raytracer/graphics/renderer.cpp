#include "renderer.hpp"
#include "math_3d/math.hpp"

#include <algorithm>
#include <execution>

#include <iostream>

namespace gfx {
    void Renderer::SetAntialiasingLevel(AntialiasingLevel level) noexcept {
        m_antialiasing_level = level;
    }

    AntialiasingLevel Renderer::GetAntialiasingLevel() const noexcept {
        return m_antialiasing_level;
    }


    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera, const gfx::Color& background) noexcept {
        const auto width = camera.GetViewportWidth(); 
        const auto height = camera.GetViewportHeight(); 
        
        if (m_frame.size() != width * height) {
            m_frame.resize(width * height);
        
        #ifdef MT
            m_vertical_it.resize(height);
            for (std::size_t i = 0; i < height; ++i) {
                m_vertical_it[i] = i;
            }

            m_horizontal_it.resize(width);
            for (std::size_t i = 0; i < width; ++i) {
                m_horizontal_it[i] = i;
            }
        #endif
        }

        gfx::Ray ray(camera.GetPositon(), math::VECTOR_BACKWARD);

        const auto dx = 2.0f / width;
        const auto dy = 2.0f / height;
        const auto aspect_ratio = (float)width / height;
        const auto fov = tanf(camera.GetFOVInRadians() / 2.0f);

    #ifdef MT
        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), 
        [this, &scene, &camera, dx, dy, aspect_ratio, fov, &ray, &background](std::uint32_t y) {
            std::for_each(std::execution::par, m_horizontal_it.cbegin(), m_horizontal_it.cend(), 
            [this, y, &scene, &camera, dx, dy, aspect_ratio, fov, &ray, &background](std::uint32_t x) {
                gfx::Color out_color = background;

                float pixel_x = (-1.0f + (x * dx)) * aspect_ratio * fov;
                float pixel_y = (1.0f - (y * dy)) * fov;
                ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();

                bool hit_anything = false;
                auto min_dist = math::MATH_INFINITY;
                for (const auto& drawable : scene.GetDrawables()) {
                    auto intersection = drawable->IsIntersect(ray);
                    if (intersection.has_value()) {
                        hit_anything = true;

                        out_color = intersection->color;
                        auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                        
                        if (int_point_dist < min_dist) {
                            float intensity = 1.0f;
                            gfx::Color color;
                            for (const auto& light : scene.GetLights()) {
                                bool valid_illum = light->ComputeIllumination(intersection->point, intersection->normal, scene.GetDrawables(), light, color, intensity);
                            }

                            // for (std::size_t s = 0; s < static_cast<std::size_t>(m_antialiasing_level); ++s) {
                            //     pixel_x = (2.0f * (x + 0.5f + math::Random(-2.0f, -2.0f)) / WIDTH - 1.0f) * FOV * WIDTH / HEIGHT;
                            //     pixel_y = -(2.0f * (y + 0.5f - math::Random(-2.0f, -2.0f)) / HEIGHT - 1.0f) * FOV;
                            //     ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();
                            //
                            //     intersection = drawable->IsIntersect(ray);
                            //     if (intersection.has_value()) {
                            //         out_color += intersection->color.ToVector4<float>();
                            //     } else {
                            //         out_color += math::vec4f(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
                            //     }
                            // }
                            //
                            // if (m_antialiasing_level != AntialiasingLevel::NONE) {
                            //     out_color /= static_cast<float>(m_antialiasing_level);
                            // }
                            
                            out_color *= intensity;
                            min_dist = int_point_dist;
                        }
                    }
                }

                m_frame[x + y * camera.GetViewportWidth()] = out_color.rgba;
            });
       });
    #else
        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                gfx::Color out_color = background;

                float pixel_x = (-1.0f + (x * dx)) * aspect_ratio * fov;
                float pixel_y = (1.0f - (y * dy)) * fov;
                ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();

                bool hit_anything = false;
                auto min_dist = math::MATH_INFINITY;
                for (const auto& drawable : scene.GetDrawables()) {
                    auto intersection = drawable->IsIntersect(ray);
                    if (intersection.has_value()) {
                        hit_anything = true;

                        out_color = intersection->color;
                        auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                        
                        if (int_point_dist < min_dist) {
                            float intensity = 1.0f;
                            gfx::Color color;
                            for (const auto& light : scene.GetLights()) {
                                bool valid_illum = light->ComputeIllumination(intersection->point, intersection->normal, scene.GetDrawables(), light, color, intensity);
                            }

                            // for (std::size_t s = 0; s < static_cast<std::size_t>(m_antialiasing_level); ++s) {
                            //     pixel_x = (2.0f * (x + 0.5f + math::Random(-2.0f, -2.0f)) / WIDTH - 1.0f) * FOV * WIDTH / HEIGHT;
                            //     pixel_y = -(2.0f * (y + 0.5f - math::Random(-2.0f, -2.0f)) / HEIGHT - 1.0f) * FOV;
                            //     ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();
                            //
                            //     intersection = drawable->IsIntersect(ray);
                            //     if (intersection.has_value()) {
                            //         out_color += intersection->color.ToVector4<float>();
                            //     } else {
                            //         out_color += math::vec4f(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
                            //     }
                            // }
                            //
                            // if (m_antialiasing_level != AntialiasingLevel::NONE) {
                            //     out_color /= static_cast<float>(m_antialiasing_level);
                            // }
                            
                            out_color *= intensity;
                            
                            min_dist = int_point_dist;
                        }
                    }
                }

                m_frame[x + y * width] = out_color.rgba;
            }
       };
    #endif

        return m_frame;
    }
}
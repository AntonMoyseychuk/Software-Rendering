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


    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera, const gfx::Color& background) noexcept {
        const auto width = camera.GetViewportWidth(); 
        const auto height = camera.GetViewportHeight(); 
        
        if (m_frame.size() != width * height) {
            m_frame.resize(width * height);

            m_vertical_it.resize(height);
            for (std::size_t i = 0; i < height; ++i) {
                m_vertical_it[i] = i;
            }
        }

        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), [this, &scene, &camera, &background, width, height](std::uint32_t y) {
            const auto dx = 2.0f / width;
            const auto dy = 2.0f / camera.GetViewportHeight();
            const auto fov = tanf(camera.GetFOVInRadians() / 2.0f);
            const auto aspect = camera.GetAspectRatio();
            gfx::Ray ray(camera.GetPositon(), math::VECTOR_BACKWARD);

            for (std::size_t x = 0; x < width; ++x) {
                // gfx::Color out_color = background;
                math::vec4f out_color = gfx::ToVector4<float>(background);

                float pixel_x = (-1.0f + (x * dx)) * aspect * fov;
                float pixel_y = (1.0f - (y * dy)) * fov;
                ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();

                bool hit_anything = false;
                auto min_dist = math::MATH_INFINITY;
                for (const auto& drawable : scene.GetDrawables()) {
                    auto intersection = drawable->IsIntersect(ray);
                    if (intersection.has_value()) {
                        hit_anything = true;

                        //out_color = intersection->color;
                        out_color = gfx::ToVector4<float>(intersection->color);
                        auto int_point_dist = (intersection->point - camera.GetPositon()).Length();
                        
                        if (int_point_dist < min_dist) {
                            float intensity = 1.0f;
                            gfx::Color light_color;
                            for (const auto& light : scene.GetLights()) {
                                bool valid_illum = light->ComputeIllumination(intersection->point, intersection->normal, scene.GetDrawables(), light, light_color, intensity);
                            }

                            //////////////////////////////////////////////////////////////////////////////////////
                            for (std::size_t s = 0; s < static_cast<std::size_t>(m_antialiasing_level); ++s) {
                                float pixel_x = (-1.0f + ((x + math::RandomInt64(-5, 5)) * dx)) * aspect * fov;
                                float pixel_y = (1.0f - ((y+ math::RandomInt64(-5, 5)) * dy)) * fov;
                                ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();
                            
                                intersection = drawable->IsIntersect(ray);
                                out_color += intersection.has_value() ? gfx::ToVector4<float>(intersection->color) : gfx::ToVector4<float>(background);
                            }
                                
                            if (m_antialiasing_level != AntialiasingLevel::NONE) {
                                out_color /= static_cast<float>(m_antialiasing_level);
                            }
                            //////////////////////////////////////////////////////////////////////////////////////

                            out_color *= intensity;
                            min_dist = int_point_dist;
                        }
                    }
                }

                // m_frame[x + y * camera.GetViewportWidth()] = out_color.rgba;
                m_frame[x + y * camera.GetViewportWidth()] = gfx::LoadColorFromVector(out_color).rgba;
            }
        });

        return m_frame;
    }
}
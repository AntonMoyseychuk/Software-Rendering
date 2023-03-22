#include "renderer.hpp"
#include "color.hpp"

#include "math_3d/math.hpp"

#include <algorithm>
#include <execution>

namespace gfx {
    void Renderer::SetAntialiasingLevel(AntialiasingLevel level) noexcept {
        m_antialiasing_level = level;
        m_antialiasing_frame_size = m_frame_size * static_cast<float>(level);
        
        m_frame.resize(m_antialiasing_frame_size.x * m_antialiasing_frame_size.y);
        
        m_vertical_it.resize(m_antialiasing_frame_size.y);
        for (std::size_t i = 0; i < m_antialiasing_frame_size.y; ++i) {
            m_vertical_it[i] = i;
        }
    }

    AntialiasingLevel Renderer::GetAntialiasingLevel() const noexcept {
        return m_antialiasing_level;
    }

    void Renderer::SetOutputFrameSize(const math::vec2ui &size) noexcept {
        if (m_frame_size.x != size.x || m_frame_size.y != size.y) {
            m_frame_size = size;
            m_antialiasing_frame_size = m_frame_size * static_cast<float>(m_antialiasing_level);
            
            m_frame.resize(m_antialiasing_frame_size.x * m_antialiasing_frame_size.y);

            m_vertical_it.resize(m_antialiasing_frame_size.y);
            for (std::size_t i = 0; i < m_antialiasing_frame_size.y; ++i) {
                m_vertical_it[i] = i;
            }
        }
    }

    const math::vec2ui &Renderer::GetOutputFrameSize() const noexcept {
        return m_frame_size;
    }

    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera, const gfx::Color& background) noexcept {
        const auto& rays = camera.GenerateRays(m_antialiasing_frame_size);
        m_frame.resize(m_antialiasing_frame_size.x * m_antialiasing_frame_size.y); // ДОЛЖНО БЫТЬ ТУТ!!!
        
        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), 
            [this, &scene, &camera, &background, rays](std::uint32_t y) {
                auto min_dist = math::MATH_INFINITY;
                std::optional<gfx::IntersectionData> closest_intersection, curr_intersection;

                for (std::size_t x = 0; x < m_antialiasing_frame_size.x; ++x) {
                    gfx::Color curr_pixel_color(0);
                    min_dist = math::MATH_INFINITY;
                    closest_intersection.reset();
                    curr_intersection.reset();

                    for (const auto& drawable : scene.GetDrawables()) {
                        if ((curr_intersection = drawable->IsIntersect(rays[x + y * m_antialiasing_frame_size.x])).has_value()) {
                            auto int_point_dist = curr_intersection->distance;
                            
                            if (int_point_dist < min_dist) {
                                closest_intersection = curr_intersection;
                                curr_pixel_color = closest_intersection->material.color;

                                float sum_intensity = 0.0f;
                                gfx::Color light_color;
                                for (const auto& light : scene.GetLights()) {
                                    bool valid_illum = light->ComputeIllumination(closest_intersection->point, 
                                        closest_intersection->normal, light_color, sum_intensity);
                                }

                                curr_pixel_color *= sum_intensity;
                                min_dist = int_point_dist;
                            }
                        }
                    }

                    if (closest_intersection.has_value() == false) {
                        curr_pixel_color = background;
                    }
                    
                    m_frame[x + y * m_antialiasing_frame_size.x] = curr_pixel_color.rgba;
                }
            }
        );


        // computing antialiasing
        const auto typed_antialiasing_level = static_cast<std::size_t>(m_antialiasing_level);
        for (std::size_t y = 0; y < m_antialiasing_frame_size.y; y += typed_antialiasing_level) {
            for (std::size_t x = 0; x < m_antialiasing_frame_size.x; x += typed_antialiasing_level) {
                math::vec4f final_color(0);
                for (std::size_t i = 0; i < typed_antialiasing_level; ++i) {
                    for (std::size_t j = 0; j < typed_antialiasing_level; ++j) {
                        final_color += gfx::ColorToVector<float>(
                            gfx::UInt32ToColor(m_frame[(x + j) + (y + i) * m_antialiasing_frame_size.x])
                        );
                    }
                }
        
                m_frame[(x / typed_antialiasing_level) + (y / typed_antialiasing_level * m_frame_size.x)] = 
                    gfx::VectorToColor(final_color / (typed_antialiasing_level * typed_antialiasing_level)).rgba;
            }
        }

        m_frame.resize(m_frame_size.x * m_frame_size.y); // ДОЛЖНО БЫТЬ ТУТ!!!
        return m_frame;
    }
}
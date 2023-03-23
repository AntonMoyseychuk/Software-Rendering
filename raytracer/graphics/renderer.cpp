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

    void Renderer::SetBackgroundColor(Color color) noexcept {
        m_background = color;
    }

    Color Renderer::GetBackgroundColor() const noexcept {
        return m_background;
    }

    gfx::Color &Renderer::GetBackgroundColor() noexcept {
        return m_background;
    }

    void Renderer::SetReflectionDepth(std::size_t depth) noexcept {
        m_reflection_depth = depth;
    }

    std::size_t Renderer::GetReflectionDepth() const noexcept {
        return m_reflection_depth;
    }

    std::size_t &Renderer::GetReflectionDepth() noexcept {
        return m_reflection_depth;
    }

    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera) noexcept {
        const auto& rays = camera.GenerateRays(m_antialiasing_frame_size);
        m_frame.resize(m_antialiasing_frame_size.x * m_antialiasing_frame_size.y); // ДОЛЖНО БЫТЬ ТУТ!!!
        
        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), 
            [this, &scene, &camera, rays](std::uint32_t y) {
                for (std::size_t x = 0; x < m_antialiasing_frame_size.x; ++x) {
                    gfx::Color curr_pixel_color = _TraceRay(rays[x + y * m_antialiasing_frame_size.x], scene, m_reflection_depth);
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
    
    Color Renderer::_TraceRay(const Ray &ray, const Scene &scene, std::size_t recursion_depth) const noexcept {
        std::optional<IntersectionData> closest_intersection;
        std::optional<IntersectionData> curr_intersection;
            
        float min_dist = math::MATH_INFINITY;
        for (const auto& drawable : scene.GetDrawables()) {
            if ((curr_intersection = drawable->IsIntersect(ray)).has_value()) {
                if (curr_intersection->distance < min_dist) {
                    closest_intersection = curr_intersection;
                    min_dist = closest_intersection->distance;
                }
            }
        }

        if (closest_intersection.has_value() == false) {
            return m_background;
        }

        float sum_intensity = 0.0f;
        for (const auto& light : scene.GetLights()) {
            light->ComputeIllumination(closest_intersection.value(), scene.GetDrawables(), sum_intensity);
        }
            
        const auto local_color = closest_intersection->material.color * sum_intensity;

        const auto& reflective_index = closest_intersection->material.reflective_index;
        if (recursion_depth == 0 || reflective_index < math::MATH_EPSILON) {
            return local_color;
        }
        
        const auto reflected_ray_dir = math::Reflect(-closest_intersection->casted_ray.direction, closest_intersection->normal);
        const auto reflected_ray = Ray(closest_intersection->point, reflected_ray_dir);

        return local_color * (1.0f - reflective_index) + _TraceRay(reflected_ray, scene, recursion_depth - 1) * reflective_index;
    }
}
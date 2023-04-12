#include "renderer.hpp"
#include "color.hpp"

#include "math_3d/math.hpp"

#include "graphics/materials/colored_material.hpp"

#include <algorithm>

namespace gfx {
    void Renderer::_TreadTileRenderFunc(std::uint32_t x0, std::uint32_t y0, std::uint32_t x_end, std::uint32_t y_end, 
        const std::vector<gfx::Ray>& rays, const gfx::Scene& scene) noexcept {
        const auto length = x_end - x0;
        
        for (std::size_t y = y0; y < y_end; ++y) {
            for (std::size_t x = x0; x < x_end; ++x) {
                m_frame[x + y * length] = _PixelShader(rays[x + y * length], scene, m_reflection_depth).rgba;
            }
        }
    }

    void Renderer::_TreadTileRawAntialiasingFunc(std::uint32_t raw_left_up_x, std::uint32_t raw_left_up_y, 
        std::uint32_t raw_right_down_x, std::uint32_t raw_right_down_y) noexcept
    {
        const auto tile_size = raw_right_down_y - raw_left_up_y;
        const auto raw_size = raw_right_down_x - raw_left_up_x;
        const float tile_pixel_num = static_cast<float>(tile_size * tile_size);

        for (std::size_t tile_start = raw_left_up_x; tile_start < raw_right_down_x; tile_start += tile_size) {
            math::vec4f final_color(0.0f);
            for (std::size_t y = 0; y < tile_size; ++y) {
                for (std::size_t x = 0; x < tile_size; ++x) {
                    final_color += gfx::ColorToVector<float>(
                        gfx::UInt32ToColor(m_frame[(raw_left_up_y + y) * raw_size + (tile_start + x)])
                    );
                }
            }

            m_frame[(tile_start / tile_size) + (raw_left_up_y / tile_size * m_frame_size.x)] = 
                    gfx::VectorToColor(final_color / tile_pixel_num).rgba;
        }
    }

    const std::vector<std::uint32_t> &Renderer::Render(const gfx::Scene &scene, const Camera& camera) noexcept {
        const auto antialiasing_frame_size = m_frame_size * static_cast<float>(m_antialiasing_level);
        m_frame.resize(antialiasing_frame_size.x * antialiasing_frame_size.y); // Must be here!!!
        
        const auto& rays = camera.GenerateRays();
        const auto step = static_cast<std::uint32_t>(m_antialiasing_level);
        for (std::uint32_t i = 0; i < antialiasing_frame_size.y; i += step) {
            m_thread_pool.AddTask(&Renderer::_TreadTileRenderFunc, this, 
                0, i, 
                antialiasing_frame_size.x, i + step, 
                std::cref(rays), 
                std::cref(scene)
            );
        }
        m_thread_pool.WaitAll();

        // computing antialiasing
        for (std::uint32_t i = 0; i < antialiasing_frame_size.y; i += step) {
            m_thread_pool.AddTask(&Renderer::_TreadTileRawAntialiasingFunc, this, 
                0, i, 
                antialiasing_frame_size.x, i + step
            );
        }
        m_thread_pool.WaitAll();

        m_frame.resize(m_frame_size.x * m_frame_size.y); // Must be here!!!
        return m_frame;
    }
    
    Color Renderer::_PixelShader(const Ray &ray, const Scene &scene, std::size_t recursion_depth) const noexcept {
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

        if (!closest_intersection.has_value()) {
            return m_background;
        }
        
        Ray scattered;
        Color attenuation;
        if (dynamic_cast<IColoredMaterial*>(closest_intersection->material.get())) {
            float sum_intensity = 0.0f;
            for (const auto& light : scene.GetLights()) {
                light->ComputeIllumination(closest_intersection.value(), scene.GetDrawables(), sum_intensity);
            }
            
            if (closest_intersection->material->Scatter(closest_intersection.value(), attenuation, scattered)) {
                const auto out_color = attenuation * sum_intensity;
                return recursion_depth == 0 ? out_color : out_color + _PixelShader(scattered, scene, recursion_depth - 1) * 0.5f;
            }
        } else {
            if (closest_intersection->material->Scatter(closest_intersection.value(), attenuation, scattered)) {
                return recursion_depth == 0 ? attenuation : attenuation * _PixelShader(scattered, scene, recursion_depth - 1);
            }
        }

        return Color::BLACK;

        // const auto local_color = closest_intersection->material.color * sum_intensity;
        // 
        // const auto& reflective_index = closest_intersection->material.reflective_index;
        // if (recursion_depth == 0 || reflective_index < math::MATH_EPSILON) {
        //     return local_color;
        // }
        // 
        // const auto reflected_ray_dir = math::Reflect(closest_intersection->casted_ray.direction, closest_intersection->normal);
        // const auto reflected_ray = Ray(closest_intersection->point, reflected_ray_dir);
        // 
        // return local_color * (1.0f - reflective_index) + _PixelShader(reflected_ray, scene, recursion_depth - 1) * reflective_index;
    }

    void Renderer::SetAntialiasingLevel(AntialiasingLevel level) noexcept {
        m_antialiasing_level = level;
        
        const auto antialiasing_frame_size = m_frame_size * static_cast<float>(level);
        m_frame.resize(antialiasing_frame_size.x * antialiasing_frame_size.y);
    }

    AntialiasingLevel Renderer::GetAntialiasingLevel() const noexcept {
        return m_antialiasing_level;
    }

    void Renderer::SetOutputFrameSize(const math::vec2ui& size) noexcept {
        if (m_frame_size.x != size.x || m_frame_size.y != size.y) {
            m_frame_size = size;
            
            const auto antialiasing_frame_size = m_frame_size * static_cast<float>(m_antialiasing_level);
            m_frame.resize(antialiasing_frame_size.x * antialiasing_frame_size.y);
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

    void Renderer::SetReflectionDepth(std::size_t depth) noexcept {
        m_reflection_depth = depth;
    }

    std::size_t Renderer::GetReflectionDepth() const noexcept {
        return m_reflection_depth;
    }
}
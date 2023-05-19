#include "renderer.hpp"

#include "math_3d/math.hpp"

#include "graphics/materials/colored_material.hpp"

#include <algorithm>

namespace raytracing::gfx {
    void Renderer::_TreadTileRenderFunc(uint32_t x0, uint32_t y0, uint32_t x_end, uint32_t y_end, 
        const std::vector<gfx::Ray>& rays, const gfx::Scene& scene) noexcept {
        const auto length = x_end - x0;
        
        for (size_t y = y0; y < y_end; ++y) {
            for (size_t x = x0; x < x_end; ++x) {
                const math::color color = _PixelShader(rays[x + y * length], scene, m_reflection_depth);
                m_frame[x + y * length] = _InternalColor(R_G_B_A(color)).rgba;
            }
        }
    }

    void Renderer::_TreadTileRawAntialiasingFunc(
        uint32_t left_up_x, uint32_t left_up_y, 
        uint32_t right_down_x, uint32_t right_down_y
    ) noexcept {
        const auto tile_size = right_down_y - left_up_y;
        const auto raw_size = right_down_x - left_up_x;
        const float tile_pixel_num = static_cast<float>(tile_size * tile_size);
        
        for (size_t tile_start = left_up_x; tile_start < right_down_x; tile_start += tile_size) {
            math::color final_color(0.0f);
            for (size_t y = 0; y < tile_size; ++y) {
                for (size_t x = 0; x < tile_size; ++x) {
                    const _InternalColor pixel_color(m_frame[(left_up_y + y) * raw_size + (tile_start + x)]);
                    final_color += math::color(pixel_color.r / 255.0f, pixel_color.g / 255.0f, pixel_color.b / 255.0f, pixel_color.a / 255.0f);
                }
            }

            m_frame[(tile_start / tile_size) + (left_up_y / tile_size * m_frame_size.x)] = _InternalColor(R_G_B_A(final_color / tile_pixel_num)).rgba;
        }
    }

    const std::vector<uint32_t> &Renderer::Render(const gfx::Scene &scene) noexcept {
        const math::vec2 antialiasing_frame_size = m_frame_size * static_cast<float>(m_antialiasing_level);
        m_frame.resize(antialiasing_frame_size.x * antialiasing_frame_size.y); // Must be here!!!
        
        const auto& rays = scene.GetCamera()->GenerateRays();
        const auto step = static_cast<uint32_t>(m_antialiasing_level);
        for (uint32_t i = 0; i < uint32_t(antialiasing_frame_size.y); i += step) {
            m_thread_pool.AddTask(&Renderer::_TreadTileRenderFunc, this, 
                0, i, 
                antialiasing_frame_size.x, i + step, 
                std::cref(rays), 
                std::cref(scene)
            );
        }
        m_thread_pool.WaitAll();

        // computing antialiasing
        for (uint32_t i = 0; i < uint32_t(antialiasing_frame_size.y); i += step) {
            m_thread_pool.AddTask(&Renderer::_TreadTileRawAntialiasingFunc, this, 
                0, i, 
                antialiasing_frame_size.x, i + step
            );
        }
        m_thread_pool.WaitAll();

        m_frame.resize(m_frame_size.x * m_frame_size.y); // Must be here!!!
        return m_frame;
    }
    
    math::color Renderer::_PixelShader(const Ray &ray, const Scene &scene, size_t recursion_depth) const noexcept {
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
        math::color attenuation;
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

        return math::color::BLACK;
    }

    void Renderer::SetAntialiasingLevel(AntialiasingLevel level) noexcept {
        m_antialiasing_level = level;
        
        const math::vec2 antialiasing_frame_size = m_frame_size * static_cast<float>(level);
        m_frame.resize(uint32_t(antialiasing_frame_size.x) * uint32_t(antialiasing_frame_size.y));
    }

    AntialiasingLevel Renderer::GetAntialiasingLevel() const noexcept {
        return m_antialiasing_level;
    }

    void Renderer::SetOutputFrameSize(const math::vec2& size) noexcept {
        if (m_frame_size.x != size.x || m_frame_size.y != size.y) {
            m_frame_size = size;
            
            const math::vec2 antialiasing_frame_size = m_frame_size * static_cast<float>(m_antialiasing_level);
            m_frame.resize(uint32_t(antialiasing_frame_size.x) * uint32_t(antialiasing_frame_size.y));
        }
    }

    const math::vec2 &Renderer::GetOutputFrameSize() const noexcept {
        return m_frame_size;
    }

    void Renderer::SetBackgroundColor(const math::color& color) noexcept {
        m_background = color;
    }

    const math::color& Renderer::GetBackgroundColor() const noexcept {
        return m_background;
    }

    void Renderer::SetReflectionDepth(size_t depth) noexcept {
        m_reflection_depth = depth;
    }

    size_t Renderer::GetReflectionDepth() const noexcept {
        return m_reflection_depth;
    }
}
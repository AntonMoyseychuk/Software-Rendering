#include "scene.hpp"
#include "math_3d/math.hpp"

namespace app {
    Scene::Scene(win_framewrk::Window *window)
        : m_window(window), m_drawables()
    {
    }
    
    void Scene::Render() const noexcept {
        const auto FOV = tanf(3.1415f / 4.0f / 2.f);
        const std::size_t WIDTH = m_window->GetWidth();
        const std::size_t HEIGHT = m_window->GetHeight();
        const auto CAMERA_POS = math::vec3f(0.0f, 0.0f, 2.5f);
        gfx::Ray ray(CAMERA_POS, math::VECTOR_BACKWARD);

        gfx::Color out_color;

        for (std::size_t y = 0; y < HEIGHT; ++y) {
            for (std::size_t x = 0; x < WIDTH; ++x) {
                float pixel_x = (2.0f * (x + 0.5f) / WIDTH - 1.0f) * FOV * WIDTH / HEIGHT;
                float pixel_y = -(2.0f * (y + 0.5f) / HEIGHT - 1.0f) * FOV;
                ray.direction = math::vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();

                out_color = gfx::Color(99, 99, 99);

                auto min_dist = math::MATH_INFINITY;
                bool hit_anything = false;
                for (const auto& drawable : m_drawables) {
                    auto intersection = drawable->IsIntersect(ray);
                    if (intersection.has_value()) {
                        hit_anything = true;
                        auto int_point_dist = (intersection->point - CAMERA_POS).Length();
                        if (int_point_dist < min_dist) {
                            float intensity = 1.0f;
                            gfx::Color color;
                            for (const auto& light : m_lights) {
                                bool valid_illum = light->ComputeIllumination(intersection->point, intersection->normal, m_drawables, light, color, intensity);
                            }

                            out_color = intersection->color * intensity;
                            min_dist = int_point_dist;
                        }
                    }
                }

                m_window->SetPixelColor(x, y, out_color.rgba);
            }
        }
    }
    
    void Scene::SetWindow(win_framewrk::Window *window) noexcept {
        assert(window != nullptr);
        m_window = window;
    }

    win_framewrk::Window *Scene::GetWindow() noexcept {
        return m_window;
    }

    const win_framewrk::Window *Scene::GetWindow() const noexcept {
        return m_window;
    }

    void Scene::AddDrawble(std::shared_ptr<gfx::IDrawable> drawable) noexcept {
        m_drawables.push_back(drawable);
    }
    
    void Scene::AddLight(std::shared_ptr<gfx::ILight> light) noexcept {
        m_lights.push_back(light);
    }
}
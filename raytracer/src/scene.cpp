#include "scene.hpp"
#include "math_3d/lin_math.hpp"

namespace app {
    Scene::Scene(win_framewrk::Window *window)
        : m_window(window), m_drawables()
    {
    }
    
    void Scene::Render() const noexcept {
        const auto FOV = tanf(3.1415f / 4.0f / 2.f);
        const std::size_t WIDTH = m_window->GetWidth();
        const std::size_t HEIGHT = m_window->GetHeight();
        const auto CAMERA_POS = math::vec4f(0.0f, 0.0f, 2.5f);
        gfx::Ray ray(CAMERA_POS, math::VECTOR_BACKWARD);

        math::vec4f int_point, int_normal;

        for (std::size_t y = 0; y < HEIGHT; ++y) {
            for (std::size_t x = 0; x < WIDTH; ++x) {
                float pixel_x = (2 * (x + 0.5f) / static_cast<float>(WIDTH) - 1) * FOV * WIDTH / static_cast<float>(HEIGHT);
                float pixel_y = -(2 * (y + 0.5f) / static_cast<float>(HEIGHT) - 1) * FOV;
                ray.direction = math::vec4f(pixel_x, pixel_y, ray.direction.z).Normalize();
                gfx::Color out_color = gfx::Color::BLACK;

                auto min_dist = INFINITY;
                for (const auto& drawable : m_drawables) {
                    auto drawable_cent_point = drawable->GetPositon() - ray.original;

                    if (drawable->IsIntersect(ray, int_point, int_normal, out_color)) {
                        // float c = math::LinMath::Dot(int_point, drawable_cent_point) / (int_point.Length() * drawable_cent_point.Length());
                        // c *= 10000;
                        // c = static_cast<std::int32_t>(c) % 500;
                        // c /= 500;

                        float intensity;
                        gfx::Color color;
                        for (const auto& light : m_lights) {
                            bool valid_illum = light->ComputeIllumination(int_point, int_normal, m_drawables, light, color, intensity);
                        }

                        auto int_point_dist = (int_point - CAMERA_POS).Length();
                        if (int_point_dist < min_dist) {
                            out_color = drawable->GetMaterial().color * intensity;
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
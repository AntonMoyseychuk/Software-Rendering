#include "math_3d/vector.hpp"
#include "math_3d/lin_math.hpp"

#include "graphics/color.hpp"
#include "graphics/ray.hpp"

#include "objects/sphere.hpp"

#include "window_framework/window.hpp"

#include <iostream>
#include <chrono>
#include <cassert>

//#define LOG_ALL

// #define TO_VIEWPORT(vector, viewport_w, viewport_h) \
//     math::Vec3f(viewport_w * (1.0f + vector.x) / 2, viewport_h * (1.0f - vector.y) / 2, vector.z)

using Sphere_t = gfx::Sphere;
using Color_t = gfx::Color;

Color_t CastRay(const gfx::Ray& ray, const std::vector<Sphere_t>& spheres) noexcept {
    assert(std::fabs(ray.direction.Length() - 1.0f) <= math::EPSILON);

    auto min_dist = INFINITY;
    Color_t out_color(0);
    math::vec4f int_point, int_normal;
    
    for (std::size_t i = 0; i < spheres.size(); ++i) {
        if (spheres[i].IsIntersect(ray, int_point, int_normal, out_color)) {
            auto dist = (spheres[i].GetPositon() - ray.original).Length();
            if (dist < min_dist) {
                min_dist = dist;
                out_color = spheres[i].GetMaterial().color;
            }
        }
    }
    
    return out_color;
}

void RenderSphere(window_framework::Window* window, const std::vector<Sphere_t>& spheres) noexcept {
    const static math::vec4f CAMERA_DIR(0.0f, 0.0f, -1.0f), RAY_ORIGIN(0.0f);
    const static float FOV = tanf(3.1415f / 4.0f / 2.f);
    gfx::Ray ray(RAY_ORIGIN, CAMERA_DIR);

    const auto _height = window->GetHeight();
    const auto _width = window->GetWidth();

    for (std::size_t y = 0; y < _height; ++y) {
        for (std::size_t x = 0; x < _width; ++x) {
            float pixel_x = (2 * (x + 0.5f) / static_cast<float>(_width) - 1) * FOV * _width / static_cast<float>(_height);
            float pixel_y = -(2 * (y + 0.5f) / static_cast<float>(_height) - 1) * FOV;
            ray.direction = math::vec4f(pixel_x, pixel_y, ray.direction.z).Normalize();

            window->SetPixelColor(x, y, CastRay(ray, spheres).rgba);
        }
    }
}

int main(int argc, char* argv[]) {
    std::uint32_t width = 480, height = 360;

    window_framework::Window* window = window_framework::Window::Get();
    auto init_res = window->Init("Raytracer", width, height);

    std::vector<Sphere_t> spheres = {
        { math::vec4f(0.0f, 0.0f, -1.5f), 0.5f, gfx::Material(Color_t::RED, 1.5f) },
        { math::vec4f(0.0f, 0.0f, -3),  0.3f, gfx::Material(Color_t::GREEN, 1.5f) },
        { math::vec4f(0.3f, 0.0f, -2),  0.3f, gfx::Material(Color_t::BLUE, 1.5f) },
        { math::vec4f(0.0f, 0.7f, -3),  0.3f, gfx::Material(Color_t::MAGENTA, 1.5f) },
        { math::vec4f(0.0f, -0.7f, -3),  0.3f, gfx::Material(Color_t::YELLOW, 1.5f) },
    };

    std::uint32_t color = 0;

    auto frame_begin = std::chrono::steady_clock::now();
    while (window->IsOpen()) {
        window->PollEvent();

        RenderSphere(window, spheres);
        
        window->PresentPixelBuffer();

        auto frame_end = std::chrono::steady_clock::now();
        LOG_WIN_INFO("FPS: " + std::to_string(1.0f / std::chrono::duration<float>(frame_end - frame_begin).count()));
        frame_begin = frame_end;

        color = window->GetPixelColor(width / 2, height / 2);
    }

    return 0;
}
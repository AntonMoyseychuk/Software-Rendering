#include "math_3d/vector.hpp"
#include "math_3d/lin_math.hpp"

#include "graphics/color.hpp"
#include "graphics/ray.hpp"

#include "objects/sphere.hpp"

#include "window_framework/window.hpp"

#include "scene.hpp"

#include <iostream>
#include <chrono>
#include <cassert>

//#define LOG_ALL

using Sphere_t = gfx::Sphere;
using Color_t = gfx::Color;

// Color_t CastRay(const gfx::Ray& ray, const std::vector<Sphere_t>& spheres, float& min_dist) noexcept {
//     assert(std::fabs(ray.direction.Length() - 1.0f) <= math::EPSILON);

//     Color_t out_color(0);
//     math::vec4f int_point, int_normal;
    

//     for (const auto& sphere : spheres) {
//         auto sphere_dist = (sphere.GetPositon() - ray.original).Length() - sphere.GetRadius();
//         if (sphere.IsIntersect(ray, int_point, int_normal, out_color)) {
//             auto int_dist = (int_point - ray.original).Length();

//             if (int_dist < min_dist) {
//                 min_dist = int_dist;
//                 out_color = sphere.GetMaterial().color - gfx::Color(((int_dist - sphere_dist) / 0.94605) * 255.0);
//             }
//         }
//     }
    
//     return out_color;
// }

// void RenderSphere(win_framewrk::Window* window, const std::vector<Sphere_t>& spheres) noexcept {
//     const static math::vec4f CAMERA_DIR(math::VECTOR_BACKWARD), RAY_ORIGIN(0.0f);
//     const static float FOV = tanf(3.1415f / 4.0f / 2.f);
//     gfx::Ray ray(RAY_ORIGIN, CAMERA_DIR);

//     const auto _height = window->GetHeight();
//     const auto _width = window->GetWidth();

//     for (std::size_t y = 0; y < _height; ++y) {
//         for (std::size_t x = 0; x < _width; ++x) {
//             float dist = INFINITY;
//             float pixel_x = (2 * (x + 0.5f) / static_cast<float>(_width) - 1) * FOV * _width / static_cast<float>(_height);
//             float pixel_y = -(2 * (y + 0.5f) / static_cast<float>(_height) - 1) * FOV;
//             ray.direction = math::vec4f(pixel_x, pixel_y, ray.direction.z).Normalize();

//             window->SetPixelColor(x, y, CastRay(ray, spheres, dist).rgba);
//         }
//     }
// }

int main(int argc, char* argv[]) {
    std::uint32_t width = 480, height = 360;

    win_framewrk::Window* window = win_framewrk::Window::Get();
    auto init_res = window->Init("Raytracer", width, height);

    app::Scene scene(window);

    auto sphere = std::make_shared<Sphere_t>(math::vec4f(0.9f, 0.5f, -3.5f), 0.8f, gfx::Material(Color_t::YELLOW, 1.5f));
    scene.AddDrawble(sphere);
    // scene.AddDrawble(std::make_shared<Sphere_t>(math::vec4f(-0.9f, -0.5f, -3.5f), 0.8f, gfx::Material(Color_t::RED, 1.5f)));

    auto frame_begin = std::chrono::steady_clock::now();
    while (window->IsOpen()) {
        window->PollEvent();

        //RenderSphere(window, spheres);
        sphere->MoveFor(math::VECTOR_BACKWARD * 0.01f);
        
        scene.Render();
        window->PresentPixelBuffer();

        auto frame_end = std::chrono::steady_clock::now();
        LOG_WIN_INFO("FPS: " + std::to_string(1.0f / std::chrono::duration<float>(frame_end - frame_begin).count()));
        frame_begin = frame_end;
    }

    return 0;
}
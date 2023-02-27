#include "math_3d/color.hpp"
#include "math_3d/vector3.hpp"
#include "math_3d/ray.hpp"
#include "math_3d/lin_math.hpp"

#include "../shapes/sphere.hpp"

#include "window_framework/window.hpp"

#include <iostream>
#include <cassert>

//#define LOG_ALL

std::uint32_t width = 1080, height = 720, R = 50;
const float FOV = 3.1415f / 4.0f;

// #define TO_VIEWPORT(vector, viewport_w, viewport_h) \
//     math::Vector3f(viewport_w * (1.0f + vector.x) / 2, viewport_h * (1.0f - vector.y) / 2, vector.z)

using vec3f = math::Vector3f;
using Sphere_t = shape::Sphere;
using Color_t = math::Color;

#define MAX_RAY_DEPTH 3 

math::Color CastRay(const math::Ray& ray, const std::vector<Sphere_t>& spheres) noexcept {
    assert(std::fabs(ray.direction.Length() - 1.0f) <= math::EPSILON);

    auto min_dist = INFINITY;
    Color_t out_color(0);
    
    for (std::size_t i = 0; i < spheres.size(); ++i) {
        if (spheres[i].IsIntersect(ray)) {
            auto dist = (spheres[i].position - ray.original).Length();
            if (dist < min_dist) {
                min_dist = dist;
                out_color = spheres[i].material.color;
            }
        }
    }

    return out_color;
}

void RenderSphere(std::vector<Color_t>& buffer,  const std::vector<Sphere_t>& spheres) noexcept {
    vec3f camera_dir(0.0f, 0.0f, -1.0f);
    math::Ray ray({0.0f}, camera_dir);

    for (std::size_t y = 0; y < height; ++y) {
        for (std::size_t x = 0; x < width; ++x) {
            float pixel_x = (2 * (x + 0.5f) / static_cast<float>(width) - 1) * tanf(FOV / 2.f) * width / static_cast<float>(height);
            float pixel_y = -(2 * (y + 0.5f) / static_cast<float>(height) - 1) * tanf(FOV /2.f);
            ray.direction = vec3f(pixel_x, pixel_y, ray.direction.z).Normalize();
            buffer[y * width + x] = CastRay(ray, spheres);
        }
    }
}

int main(int argc, char* argv[]) {
    window_framework::Window* window = window_framework::Window::Get();
    auto init_res = window->Init("Raytracer", width, height);

    std::vector<Sphere_t> spheres = {
        { vec3f(-0.3f, 0.0f, -4), 0.3f, gfx::Material(Color_t::YELLOW, 1.5f) },
        { vec3f(0.0f, 0.0f, -3),  0.3f, gfx::Material(Color_t::MAGENTA, 1.5f) },
        { vec3f(0.3f, 0.0f, -2),  0.3f, gfx::Material(Color_t::GREEN, 1.5f) },
    };

    std::vector<Color_t> buffer(width * height);

    while (window->IsOpen()) {
        window->PollEvent();

        width = window->GetSDLSurface()->w;
        height = window->GetSDLSurface()->h;
        if (buffer.size() != width * height) {
            buffer.resize(width * height);
        }

        RenderSphere(buffer, spheres);
        
        window->FillPixelBuffer(buffer);
        window->PresentPixelBuffer();
    }

    return 0;
}
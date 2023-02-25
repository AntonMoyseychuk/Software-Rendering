#include "math_3d/color.hpp"
#include "math_3d/vector3.hpp"
#include "math_3d/lin_math.hpp"

#include "shapes/sphere.hpp"

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

math::Color CastRay(const vec3f& orig, const vec3f& dir, const std::vector<Sphere_t>& spheres, const std::vector<Color_t>& sphere_colors) noexcept {
    for (std::size_t i = 0; i < spheres.size(); ++i) {
        if (spheres[i].IsRayIntersect(orig, dir)) {
            return sphere_colors[i];
        }
    }

    return Color_t(0); //
}

void RenderSphere(std::vector<Color_t>& buffer,  const std::vector<Sphere_t>& spheres, const std::vector<Color_t>& sphere_colors) noexcept {
    assert(sphere_colors.size() == spheres.size());

    math::Vector3f camera_pos(0.0f), camera_dir(0.0f, 0.0f, -1.0f);

    for (std::size_t y = 0; y < height; ++y) {
        for (std::size_t x = 0; x < width; ++x) {
            float pixel_x = (2 * (x + 0.5f) / static_cast<float>(width) - 1) * tanf(FOV / 2.f) * width / static_cast<float>(height);
            float pixel_y = -(2 * (y + 0.5f) / static_cast<float>(height) - 1) * tanf(FOV /2.f);
            auto dir = vec3f(pixel_x, pixel_y, camera_dir.z).Normalize();
            buffer[y * width + x] = CastRay(camera_pos, dir, spheres, sphere_colors);
        }
    }
}

int main(int argc, char* argv[]) {
    window_framework::Window* window = window_framework::Window::Get();
    auto init_res = window->Init("raytracer", width, height);

    std::vector<Sphere_t> spheres = {
        { vec3f(0.f, 0.f, -2), 0.25f },
        { vec3f(-0.6f, 0.f, -2), 0.2f },
        { vec3f(0.5f, -0.5f, -2), 0.7f },
    };
    std::vector<Color_t> colors = {
        Color_t::YELLOW, Color_t::MAGENTA, Color_t::GREEN
    };

    std::vector<Color_t> buffer(width * height);

    while (window->IsOpen()) {
        window->PollEvent();

        width = window->GetSDLSurface()->w;
        height = window->GetSDLSurface()->h;
        if (buffer.size() != width * height) {
            buffer.resize(width * height);
        }

        RenderSphere(buffer, spheres, colors);
        
        window->FillPixelBuffer(buffer);
        window->PresentPixelBuffer();
    }

    return 0;
}
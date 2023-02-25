#include "math_3d/color.hpp"
#include "math_3d/vector3.hpp"
#include "math_3d/lin_math.hpp"

#include "shapes/sphere.hpp"

#include "window_framework/window.hpp"

#include <limits>

std::uint32_t width = 1080, height = 720, R = 50;
const float FOV = 3.1415f / 4.0f;

#define TO_VIEWPORT(vector, viewport_w, viewport_h) \
    math::Vector3f(viewport_w * (1.0f + vector.x) / 2, viewport_h * (1.0f - vector.y) / 2, vector.z)

math::Color CastRay(const math::Vector3f& orig, const math::Vector3f& dir, const shape::Sphere& sphere, const math::Color& sphere_color) noexcept {
    return sphere.IsRayIntersect(orig, dir) ? sphere_color : math::Color(0);
}

void RenderSphere(std::vector<math::Color>& buffer, shape::Sphere& sphere, const math::Color& color) noexcept {
    math::Vector3f camera_pos(0.0f), camera_dir(0.0f, 0.0f, -1.0f);

    for (std::size_t y = 0; y < height; ++y) {
        for (std::size_t x = 0; x < width; ++x) {
            float pixel_x = (2 * (x + 0.5) / static_cast<float>(width) - 1) * tanf(FOV / 2.f) * width / static_cast<float>(height);
            float pixel_y = -(2 * (y + 0.5) / static_cast<float>(height) - 1) * tanf(FOV /2.f);
            auto dir = math::Vector3f(pixel_x, pixel_y, camera_dir.z).Normalize();
            buffer[y * width + x] = CastRay(camera_pos, dir, sphere, color);
        }
    }
}

int main(int argc, char* argv[]) {
    window_framework::Window* window = window_framework::Window::Get();
    auto init_res = window->Init("raytracer", width, height);

    shape::Sphere sphere(math::Vector3f(0.f, 0.f, -2), 0.3f);

    using Color_t = math::Color;
    std::vector<Color_t> buffer(width * height);

    while (window->IsOpen()) {
        window->PollEvent();

        width = window->GetSDLSurface()->w;
        height = window->GetSDLSurface()->h;
        if (buffer.size() != width * height) {
            buffer.resize(width * height);
        }

        RenderSphere(buffer, sphere, math::Color::YELLOW);     
        
        window->FillPixelBuffer(buffer);
        window->PresentPixelBuffer();
    }

    return 0;
}
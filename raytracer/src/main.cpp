#include "math_3d/color.hpp"
#include "math_3d/vector.hpp"

#include "window_framework/window.hpp"


#define TO_VIEWPORT(vector, viewport_w, viewport_h) \
    math::Vector3(viewport_w * (1.0f + vector.x) / 2, viewport_h * (1.0f - vector.y) / 2, vector.z)

int main(int argc, char* argv[]) {
    std::uint32_t width = 1080, height = 720, R = 50;

    window_framework::Window* window = window_framework::Window::Get();
    auto init_res = window->Init("raytracer", width, height);

    math::Vector3 circle_pos(0);

    using Color_t = math::Color;
    std::vector<Color_t> buffer(width * height);

    while (window->IsOpen()) {
        window->PollEvent();

        width = window->GetSDLSurface()->w;
        height = window->GetSDLSurface()->h;
        auto viewport_pos = TO_VIEWPORT(circle_pos, width, height);

        for(std::size_t y = 0; y < height; ++y) {
           for (std::size_t x = 0; x < width; ++x) {
               buffer[x + y * width] = ((viewport_pos.x - x) * (viewport_pos.x - x) + (viewport_pos.y - y) * (viewport_pos.y - y) <= R * R) ? 
                  Color_t::YELLOW : Color_t::BLACK;
           }
        }
        
        window->FillPixelBuffer(buffer);
        window->PresentPixelBuffer();
    }

    return 0;
}
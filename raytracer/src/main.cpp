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


int main(int argc, char* argv[]) {
    std::uint32_t width = 480, height = 360;

    win_framewrk::Window* window = win_framewrk::Window::Get();
    auto init_res = window->Init("Raytracer", width, height);

    app::Scene scene(window);

    auto sphere = std::make_shared<Sphere_t>(math::vec4f(0.0f, 0.0f, -3.5f), 1.0f, gfx::Material(Color_t::YELLOW, 1.5f));
    scene.AddDrawble(sphere);

    auto frame_begin = std::chrono::steady_clock::now();
    while (scene.GetWindow()->IsOpen()) {
        scene.GetWindow()->PollEvent();
        
        scene.Render();
        scene.GetWindow()->PresentPixelBuffer();
        
        // sphere->MoveFor(math::VECTOR_BACKWARD * 0.01f);

        auto frame_end = std::chrono::steady_clock::now();
        LOG_WIN_INFO("FPS: " + std::to_string(1.0f / std::chrono::duration<float>(frame_end - frame_begin).count()));
        frame_begin = frame_end;
    }

    return 0;
}
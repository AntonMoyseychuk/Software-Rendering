#include "math_3d/vector.hpp"
#include "math_3d/math.hpp"

#include "graphics/color.hpp"
#include "graphics/ray.hpp"

#include "objects/sphere.hpp"
#include "objects/point_light.hpp"

#include "window_framework/window.hpp"

#include "scene.hpp"

#include <iostream>
#include <chrono>
#include <cassert>

//#define LOG_ALL

using Sphere_t = gfx::Sphere;
using Color_t = gfx::Color;


int main(int argc, char* argv[]) {
    std::uint32_t width = 720, height = 480;

    win_framewrk::Window* window = win_framewrk::Window::Get();
    auto init_res = window->Init("Raytracer", width, height);

    app::Scene scene(window);

    scene.AddDrawble(std::make_shared<Sphere_t>(math::vec3f(0.0f), 0.7f, gfx::Material(Color_t::RED, 1.5f)));
    scene.AddDrawble(std::make_shared<Sphere_t>(math::VECTOR_RIGHT * 2 + math::VECTOR_BACKWARD * 3, 0.7f, gfx::Material(Color_t::YELLOW, 1.5f)));
    scene.AddDrawble(std::make_shared<Sphere_t>(math::VECTOR_LEFT * 2 + math::VECTOR_BACKWARD * 3 + math::VECTOR_UP, 0.7f, gfx::Material(Color_t::CYAN, 1.5f)));

    auto light = std::make_shared<gfx::PointLigth>(math::vec3f(2.0f, -2.0f, 5.0f), gfx::Color::WHITE, 1.0f);
    scene.AddLight(light);

    auto frame_begin = std::chrono::steady_clock::now();
    while (scene.GetWindow()->IsOpen()) {
        scene.GetWindow()->PollEvent();
        
        SDL_PumpEvents();
        auto keyboard_state = SDL_GetKeyboardState(nullptr);
        if (keyboard_state[SDL_Scancode::SDL_SCANCODE_A]) {
            light->MoveFor(math::VECTOR_LEFT * 0.1f);
        }
        if (keyboard_state[SDL_Scancode::SDL_SCANCODE_W]) {
            light->MoveFor(math::VECTOR_UP * 0.1f);
        }
        if (keyboard_state[SDL_Scancode::SDL_SCANCODE_D]) {
            light->MoveFor(math::VECTOR_RIGHT * 0.1f);
        }
        if (keyboard_state[SDL_Scancode::SDL_SCANCODE_S]) {
            light->MoveFor(math::VECTOR_DOWN * 0.1f);
        }
        if (keyboard_state[SDL_Scancode::SDL_SCANCODE_UP]) {
            light->MoveFor(math::VECTOR_BACKWARD * 0.1f);
        }
        if (keyboard_state[SDL_Scancode::SDL_SCANCODE_DOWN]) {
            light->MoveFor(math::VECTOR_FORWARD * 0.1f);
        }

        scene.Render();
        scene.GetWindow()->PresentPixelBuffer();

        auto frame_end = std::chrono::steady_clock::now();
        LOG_WIN_INFO("FPS: " + std::to_string(1.0f / std::chrono::duration<float>(frame_end - frame_begin).count()));
        frame_begin = frame_end;
    }

    return 0;
}
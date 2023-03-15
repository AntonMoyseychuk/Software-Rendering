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

//#define LOG_ALL

#ifdef _DEBUG
    #define LOG_EXPR(expresion) std::cout << "{" << #expresion << "}: " << (expresion) << std::endl
#endif

int main(int argc, char* argv[]) {
    std::uint32_t width = 720, height = 480;

    win_framewrk::Window* window = win_framewrk::Window::Get();
    auto init_res = window->Init("Raytracer", width, height);
    window->SetBackgroundColor(gfx::Color(100, 100, 100).rgba);

    app::Scene scene(window);

    scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.0f, -1.0f), 2.0f, gfx::Material(gfx::Color::RED, 1.5f)));
    // scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(2.0f, 2.0f, -4.0f), 0.5f, gfx::Material(gfx::Color::YELLOW, 1.5f)));
    // scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-3.0f, 1.0f, -3.0f), 0.5f, gfx::Material(gfx::Color::CYAN, 1.5f)));
    // scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(3.0f, -5.0f, -10.0f), 0.5f, gfx::Material(gfx::Color::MAGENTA, 1.5f)));
    // scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-3.0f, 5.0f, -10.0f), 0.5f, gfx::Material(gfx::Color::BLUE, 1.5f)));

    auto light = std::make_shared<gfx::PointLigth>(math::vec3f(0.0f, 0.0f, 5.0f), gfx::Color::WHITE, 1.0f);
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
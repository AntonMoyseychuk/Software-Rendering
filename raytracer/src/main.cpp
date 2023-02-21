#include "math_3d/color.hpp"
#include "math_3d/vector.hpp"

#include <SDL2/SDL.h>
#include <thread>
#include <iostream>

#define MAP_RGBA(format, color) SDL_MapRGBA(format, color.r, color.g, color.b, color.a)
#define TO_VIEWPORT(vector, viewport_w, viewport_h) \
    math::Vector3(viewport_w * (1.0f + vector.x) / 2, viewport_h * (1.0f - vector.y) / 2, vector.z)

void ThreadDrawFun(SDL_Window*& window, bool& is_quit, const math::Vector3& circle_pos) {
    auto surface = SDL_GetWindowSurface(window);
    std::uint32_t* buffer = static_cast<std::uint32_t*>(surface->pixels);
    std::size_t width = surface->w, height = surface->h;

    const float r = 50;

    using Color_t = math::Color;
    Color_t colors[] = { 
        Color_t::RED, Color_t::GREEN, Color_t::BLUE, 
        Color_t::YELLOW, Color_t::MAGENTA, Color_t::CYAN, 
        Color_t::WHITE 
    };
    std::size_t curr_color = 0;

    while(!is_quit) {
        auto viewport_pos = TO_VIEWPORT(circle_pos, width, height);
        std::cout << '[' << viewport_pos.x << ',' << viewport_pos.y << ']' << std::endl;

        for(std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                buffer[x + y * width] = ((viewport_pos.x - x) * (viewport_pos.x - x) + (viewport_pos.y - y) * (viewport_pos.y - y) <= r * r) ? 
                    MAP_RGBA(surface->format, colors[curr_color]) : MAP_RGBA(surface->format, Color_t::BLACK);
            }
        }

        SDL_UpdateWindowSurface(window);
        // (curr_color += 1) %= SDL_arraysize(colors);
        // SDL_Delay(500);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    auto window = SDL_CreateWindow("raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, 0);

    SDL_Event event;
    bool is_quit = false;
    math::Vector3 circle_pos(0);
    const std::uint8_t* keyboard_state = nullptr;

    std::thread draw_thread(ThreadDrawFun, std::ref(window), std::ref(is_quit), std::ref(circle_pos));

    while (!is_quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type)  {
            case SDL_QUIT:
                is_quit = true;
                break;
            case SDL_KEYDOWN:
                keyboard_state = SDL_GetKeyboardState(nullptr);
                if (keyboard_state[SDL_SCANCODE_A]) {
                    circle_pos += math::Vector3::LEFT / 100.0f;
                }
                if (keyboard_state[SDL_SCANCODE_W]) {
                    circle_pos += math::Vector3::UP / 100.0f;
                }
                if (keyboard_state[SDL_SCANCODE_D]) {
                    circle_pos += math::Vector3::RIGHT / 100.0f;
                }
                if (keyboard_state[SDL_SCANCODE_S]) {
                    circle_pos += math::Vector3::DOWN / 100.0f;
                }
                break;
            }
        }
    }

    draw_thread.join();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
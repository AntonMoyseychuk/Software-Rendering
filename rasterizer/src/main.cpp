#include "math_3d/lin_math.hpp"
#include "math_3d/mat3x3.hpp"
#include "math_3d/vector.hpp"
#include "math_3d/color.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <Windows.h>

#define ASSERT_SDL_CALL(expr, ...) if (!(expr)) { \
    std::cerr << SDL_GetError() << std::endl; \
    __VA_ARGS__; \
    exit(EXIT_FAILURE); \
}

#define TO_RASTER(screen_w, screen_h, v) \
    math::Vector3((screen_w) / 2 * (1.0f + v.x), (screen_h) / 2 * (1.0f - v.y), v.z)

int main( int argc, char* argv[] ) {
    using namespace math;
    
    const std::uint32_t w = 720, h = 480;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface* surface = nullptr;

    ASSERT_SDL_CALL(SDL_Init(SDL_INIT_VIDEO) == 0);

    window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
    ASSERT_SDL_CALL(window != nullptr, SDL_Quit());

    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ASSERT_SDL_CALL(renderer != nullptr, 
        SDL_DestroyWindow(window);
        SDL_Quit()
    );
        
    surface = SDL_GetWindowSurface(window);
    ASSERT_SDL_CALL(surface != nullptr, 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit()
    );
    
    Vector3 v0(-0.5, -0.5, 1.0);
    Vector3 v1( 0.0,  0.5, 1.0);
    Vector3 v2( 0.5, -0.5, 1.0);

    v0 = TO_RASTER(w, h, v0);
    v1 = TO_RASTER(w, h, v1);
    v2 = TO_RASTER(w, h, v2);

    auto M = LinMath::Transpose(Mat3x3(v0, v1, v2));

    try {
        M = LinMath::Inverse(M);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    auto E0 = Vector3(1, 0, 0) * M;
    auto E1 = Vector3(0, 1, 0) * M;
    auto E2 = Vector3(0, 0, 1) * M;

    std::uint32_t* frameBuffer = (std::uint32_t*)surface->pixels;

    for (std::size_t y = 0; y < h; y++) {
        for (std::size_t x = 0; x < w; x++) {
            math::Vector3 sample = { x + 0.5f, y + 0.5f, 1.0f };

            float alpha = LinMath::Dot(E0, sample);
            float beta = LinMath::Dot(E1, sample);
            float gamma = LinMath::Dot(E2, sample);

            if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) {
                #define BLENDING
                #ifdef BLENDING
                    Color result_color(Color::CYAN * alpha + Color::YELLOW * beta + Color::MAGENTA * gamma);
                    frameBuffer[x + y * w] = SDL_MapRGB(surface->format, result_color.r,  result_color.g,  result_color.b);
                #else
                    frameBuffer[x + y * w] = SDL_MapRGB(surface->format, Color::CYAN.r,  Color::CYAN.g,  Color::CYAN.b);
                #endif
            }
        }
    }

    bool is_quit = false;
    while (!is_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)  {
            case SDL_QUIT:
                is_quit = true;
                break;
            }
        } 

        SDL_UpdateWindowSurface(window);
    }
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
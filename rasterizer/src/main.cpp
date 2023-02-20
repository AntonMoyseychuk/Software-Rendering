// #include "math/lin_math.hpp"
// #include "math/mat3x3.hpp"
// #include "math/vector.hpp"
// #include "math/color.hpp"

// #include <iostream>
// #include <algorithm>
// #include <vector>
// #include <fstream>

// static const std::uint16_t w = 720, h = 480;

// #define TO_RASTER(screen_w, screen_h, v) \
//     math::Vector3((screen_w) / 2 * (1.0f + v.x), (screen_h) / 2 * (1.0f - v.y), v.z)


// void OutputFrame(const std::vector<math::Color>& frameBuffer, const char* filename) {
//     std::ofstream file(filename, std::ios_base::binary);
//     file << "P3\n" << w << " " << h << "\n" << 255 << "\n ";
//     for (std::size_t i = 0; i < frameBuffer.size() ; ++i) {
//         file << static_cast<std::uint32_t>(frameBuffer[i].r) << " " 
//             << static_cast<std::uint32_t>(frameBuffer[i].g) << " " 
//             << static_cast<std::uint32_t>(frameBuffer[i].b) << " ";
//     }
// }


// int main(int argc, char* argv[]) {
//     using namespace math;
    
//     Vector3 v0(-0.5, -0.5, 1.0);
//     Vector3 v1( 0.0,  0.5, 1.0);
//     Vector3 v2( 0.5, -0.5, 1.0);

//     v0 = TO_RASTER(w, h, v0);
//     v1 = TO_RASTER(w, h, v1);
//     v2 = TO_RASTER(w, h, v2);

//     auto M = LinMath::Transpose(Mat3x3(v0, v1, v2));

//     try {
//         M = LinMath::Inverse(M);
//     }
//     catch(const std::exception& e) {
//         std::cerr << e.what() << '\n';
//         return -1;
//     }

//     math::Vector3 E0 = math::Vector3(1, 0, 0) * M;
//     math::Vector3 E1 = math::Vector3(0, 1, 0) * M;
//     math::Vector3 E2 = math::Vector3(0, 0, 1) * M;

//     std::vector<math::Color> frameBuffer(w * h);

//     for (auto y = 0; y < h; y++) {
//         for (auto x = 0; x < w; x++) {
//             math::Vector3 sample = { x + 0.5f, y + 0.5f, 1.0f };

//             float alpha = LinMath::Dot(E0, sample);
//             float beta = LinMath::Dot(E1, sample);
//             float gamma = LinMath::Dot(E2, sample);

//             if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) {
//                 #define BLENDING
//                 #ifdef BLENDING
//                     frameBuffer[x + y * w] = Color(Color::CYAN * alpha + Color::YELLOW * beta + Color::MAGENTA * gamma);
//                 #else
//                     frameBuffer[x + y * w] = Color::YELLOW;
//                 #endif
//             }
//         }
//     }

//     OutputFrame(frameBuffer, "D:\\Studies\\Graphics\\software_rendering\\rasterizer\\out\\triangle.ppm");

//     return 0;
// }

    #include <SDL2/SDL.h>
    #include <SDL2/SDL_main.h> // only include this one in the source file with main()!

    int main( int argc, char* argv[] )
    {
        const int WIDTH = 640;
        const int HEIGHT = 480;
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
        renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
#include "application.hpp"

// #include <iostream>
// #define CLOCK_NOW() std::chrono::steady_clock::now()
// #define DURATION(begin, end) std::chrono::duration<float, std::milli>((end) - (begin)).count()

// #include "math_3d/mat_operations.hpp"
// #include "math_3d/vec_operations.hpp"
// #include "math_3d/util.hpp"

int main(int argc, char* argv[]) {
    rasterization::Application app("rasterizer", 720, 640, SIZE_MAX);
    app.Run();

    // auto begin = CLOCK_NOW();
    // for (size_t i = 0; i < 10'000'000; ++i) {
    //    
    // }
    // std::cout << DURATION(begin, CLOCK_NOW()) << "ms\n";
    //
    // for (size_t i = 0; i < 10'000'000; ++i) {
    //    
    // }
    // std::cout << DURATION(begin, CLOCK_NOW()) << "ms\n";

    return 0;
}
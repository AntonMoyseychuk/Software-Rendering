#include "application.hpp"

#include <iostream>
#define CLOCK_NOW() std::chrono::steady_clock::now()
#define DURATION(begin, end) std::chrono::duration<float, std::milli>((end) - (begin)).count()

int main(int argc, char* argv[]) {
    rasterization::Application app("rasterizer", 720, 640, 60);
    app.Run();
    return 0;
}
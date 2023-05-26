#include "application.hpp"

#include <iostream>
#define CLOCK_NOW() std::chrono::steady_clock::now()
#define DURATION(begin, end) std::chrono::duration<float, std::milli>((end) - (begin)).count()

// #include "math_3d/mat_operations.hpp"
// #include "math_3d/util.hpp"

int main(int argc, char* argv[]) {
    rasterization::Application app("rasterizer", 720, 640, SIZE_MAX);
    app.Run();

    // const auto local = math::vec3f::RIGHT;
    // const auto world = local * math::translate(math::mat4f::IDENTITY, math::vec3f::RIGHT);
    // const auto camera = world * math::look_at_rh(math::vec3f::FORWARD, math::vec3f::BACKWARD, math::vec3f::UP);
    // const auto perspective = camera * math::perspective(math::to_radians(90.0f), 800.0f / 600.0f, 1.0f, 100.0f);

    // auto begin = CLOCK_NOW();
    // for (size_t i = 0; i < 10'000'000; ++i) {
    //     math::look_at_rh(math::vec3f::FORWARD, math::vec3f::BACKWARD, math::vec3f::UP);
    // }
    // std::cout << DURATION(begin, CLOCK_NOW()) << "ms\n";

    return 0;
}
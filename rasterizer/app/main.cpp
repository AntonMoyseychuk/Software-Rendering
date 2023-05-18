#include "application.hpp"
#include "math_3d/vec2.hpp"
#include "math_3d/vec3.hpp"
#include "math_3d/vec4.hpp"
#include "math_3d/mat4.hpp"
#include "math_3d/mat_op.hpp"

#include <iostream>
#define CLOCK_NOW() std::chrono::steady_clock::now()
#define DURATION(begin, end) std::chrono::duration<float, std::milli>((end) - (begin)).count()




int main(int argc, char* argv[]) {
    rasterization::Application app("rasterizer", 720, 640, 60);
    app.Run();
    return 0;
}
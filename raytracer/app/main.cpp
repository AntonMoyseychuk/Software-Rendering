#include "application.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    // app::Application app("Raytracer", 640, 480);
    // app.Run();

    const auto q = (math::Quaternion(cos(math::ToRadians(45.0f)), 0.0f, 0.0f, cos(math::ToRadians(45.0f))));
    const math::vec3f v = math::VECTOR_UP;
    const auto rotated = v * q;

    std::cout << "v: (" << v.x << ", " << v.y << ", " << v.z << ")\n";
    std::cout << "rotated: (" << rotated.x << ", " << rotated.y << ", " << rotated.z << ")\n";
    return 0;
}
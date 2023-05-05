#include "application.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    raytracing::Application app("Raytracer", 640, 480);
    app.Run();
    return 0;
}
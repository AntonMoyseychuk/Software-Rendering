#include "application.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    app::Application app("Raytracer", 640, 480);
    app.Run();
    return 0;
}
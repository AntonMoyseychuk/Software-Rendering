#include "application.hpp"

int main(int argc, char* argv[]) {
    raytracing::app::Application app("Raytracer", 640, 480);
    app.Run();
    return 0;
}
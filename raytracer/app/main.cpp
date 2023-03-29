#include "application.hpp"

int main(int argc, char* argv[]) {
    app::Application app("Raytracer", 540, 400);
    app.Run();
    return 0;
}
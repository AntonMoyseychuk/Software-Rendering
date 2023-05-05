#include "application.hpp"

int main(int argc, char* argv[]) {
    rasterization::Application app("rasterizer", 720, 640, 10);
    app.Run();
    return 0;
}
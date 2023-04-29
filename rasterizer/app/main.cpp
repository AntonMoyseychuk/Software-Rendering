#include "application.hpp"

int main(int argc, char* argv[]) {
    std::unique_ptr<app::Application> app = std::make_unique<app::Application>("rasterizer", 720, 640);
    app->Run();
    return 0;
}
#include "application.hpp"

int main(int argc, char* argv[]) {
    std::unique_ptr<app::Application> app = std::make_unique<app::Application>("rasterizer", 540, 480);
    app->Run();
    return 0;
}
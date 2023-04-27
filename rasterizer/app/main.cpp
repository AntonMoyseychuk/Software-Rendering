#include "window/window.hpp"

int main(int argc, char* argv[]) {
    auto window = win_framewrk::Window::Get();
    window->Init("rasterization", 640, 480);

    window->SetBackgroundColor(0xff00ffff);
    while(window->IsOpen()) {
        window->PollEvent();
        window->PresentPixelBuffer();
    }
    return 0;
}
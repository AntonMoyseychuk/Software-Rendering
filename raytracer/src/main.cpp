#include "application.hpp"

#ifdef _DEBUG
    #define LOG_EXPR(expresion) std::cout << "{" << #expresion << "}: " << (expresion) << std::endl
#endif

int main(int argc, char* argv[]) {
    app::Application app("Raytracer", 540, 400);
    app.Run();
    return 0;
}
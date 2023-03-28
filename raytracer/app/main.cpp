#include "application.hpp"

#ifdef _DEBUG
    #include <iostream>
    #define LOG_EXPR(expresion) std::cout << "{" << #expresion << "}: " << (expresion) << std::endl
#else 
    #define LOG_EXPR(expresion) (void)0
#endif

int main(int argc, char* argv[]) {
    app::Application app("Raytracer", 540, 400);
    app.Run();
    return 0;
}
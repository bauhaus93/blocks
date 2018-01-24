
#include <iostream>

#include "Application.hpp"

int main(int argc, char** argv) {
    mc::Application app {};

    try {
        app.Loop();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "[UNHANDLED EXCEPTION] " << e.what() << std::endl;
    }

    return 0;
}

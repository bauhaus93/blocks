
#include <iostream>
#include <memory>

#include "logger/GlobalLogger.hpp"
#include "Application.hpp"

int main(int argc, char** argv) {
    mc::log::CreateGlobalLogger(std::cout);

    auto app = std::make_unique<mc::Application>(800, 600);

    try {
        app->Loop();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "[UNHANDLED EXCEPTION] " << e.what() << std::endl;
    }

    mc::log::DestroyGlobalLogger();

    return 0;
}

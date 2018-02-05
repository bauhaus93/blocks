
#include <iostream>
#include <memory>

#include "logger/GlobalLogger.hpp"
#include "Application.hpp"
#include "ApplicationError.hpp"

int main(int argc, char** argv) {
    mc::log::CreateGlobalLogger(std::cout);

    try {
        mc::Application app { 800, 600 };
        app.Loop();
    }
    catch (const mc::ApplicationError& e) {
        ERROR(e.what());
    }
    
    mc::log::DestroyGlobalLogger();

    return 0;
}

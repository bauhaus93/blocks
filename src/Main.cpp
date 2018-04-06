
#include <iostream>
#include <fstream>

#include "logger/GlobalLogger.hpp"
#include "Application.hpp"
#include "ApplicationError.hpp"

int main(int argc, char** argv) {
    //std::ofstream out("out.log");
    mc::log::CreateGlobalLogger(std::cout);

    try {
        mc::Application app { 1024, 768 };
        app.Run();
    }
    catch (const mc::ApplicationError& e) {
        ERROR(e.what());
    }

    mc::log::DestroyGlobalLogger();

    return 0;
}

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "logger/GlobalLogger.hpp"
#include "application/Application.hpp"
#include "application/ApplicationError.hpp"

int main(int argc, char** argv) {
    //std::ofstream out("out.log");
    blocks::CreateGlobalLogger(std::cout);

    try {
        blocks::Application app { 1024, 768 };
        app.Run();
    } catch (const blocks::ApplicationError& e) {
        ERROR(e.what());
    } catch (const std::exception& e) {
        ERROR("Unhandled exception: ", e.what());
    }

    blocks::DestroyGlobalLogger();

    return 0;
}

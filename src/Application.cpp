/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Application.hpp"

namespace mc {

Application::Application() {
    log::CreateGlobalLogger(std::cout);
    INFO("Initialized Logger");
}

Application::~Application() {
    INFO("Closing Logger");
    log::DestroyGlobalLogger();
}

void Application::Loop() {

}

}   // namespace mc

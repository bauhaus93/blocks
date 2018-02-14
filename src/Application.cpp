/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Application.hpp"

namespace mc {

static bool LoadGlad();

Application::Application(unsigned int winX, unsigned int winY):
    window { sf::VideoMode(winX, winY),
            "MC",
            sf::Style::Default,
            sf::ContextSettings(24, 8, 4, 4, 6) },
    stateStack { window } {
    INFO("Creating application");

    LoadGlad();

    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    window.setMouseCursorVisible(false);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

Application::~Application() {
    INFO("Destroying application");
}

void Application::Run() {
    INFO("Running application");
    stateStack.Push(State::WORLD);
    stateStack.Run();
}

static bool LoadGlad() {
    INFO("Loading glad");
    if (!gladLoadGL()) {
        throw OpenGLError("gladLoadGL", __FUNCTION__);
    }
    DEBUG("Glad loaded");
    return true;
}

}   // namespace mc

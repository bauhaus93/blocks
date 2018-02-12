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

    LoadGlad();

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

Application::~Application() {

}

void Application::Run() {
    stateStack.Push(State::WORLD);
    stateStack.Run();
}

static bool LoadGlad() {
    if (!gladLoadGL()) {
        throw OpenGLError("gladLoadGL", __FUNCTION__);
    }
    INFO("Glad loaded");
    return true;
}

}   // namespace mc

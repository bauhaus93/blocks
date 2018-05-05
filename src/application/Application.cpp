/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Application.hpp"

namespace mc {

static bool LoadGlad();

Application::Application(unsigned int winX, unsigned int winY):
    window { sf::VideoMode(winX, winY),
            "MC",
            sf::Style::Default,
            sf::ContextSettings(24, 8, 4, 4, 2) },
    stateStack { window } {
    INFO("Creating application");
    sf::ContextSettings settings = window.getSettings();
    if (settings.majorVersion < 4 ||
        (settings.majorVersion == 4 && settings.minorVersion < 2)) {
        throw ApplicationError("OpenGL version",
                               __FUNCTION__,
                               "Needed at least opengl version 4.2, but got only " +
                               std::to_string(settings.majorVersion) + "." + std::to_string(settings.minorVersion));
    }
    LoadGlad();

    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    window.setMouseCursorVisible(false);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glClearDepth(1.0);
    //glFrontFace(GL_CCW); //default
    //glCullFace(GL_BACK); //default
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
        throw graphics::OpenGLError("gladLoadGL", __FUNCTION__);
    }
    DEBUG("Glad loaded");
    return true;
}

}   // namespace mc

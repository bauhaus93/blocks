/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Application.hpp"

namespace mc {

Application::Application(unsigned int winX, unsigned int winY):
    active { true },
    window { sf::VideoMode(winX, winY),
            "MC",
            sf::Style::Default,
            sf::ContextSettings(24, 8, 4, 4, 6) },
    vao { 0 } {

    if (!gladLoadGL()) {
        throw OpenGLError("gladLoadGL", __FUNCTION__);
    }

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

}

Application::~Application() {
    log::DestroyGlobalLogger();
}

void Application::Loop() {
    INFO("Starting application main loop");
    while (active) {

        DrawScene();
        HandleEvents();
    }
    INFO("Finishing application main loop");
}

void Application::HandleEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            active = false;
        } else if (event.type == sf::Event::Resized) {
           glViewport(0, 0, event.size.width, event.size.height);
        }
    }
}

void Application::DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window.display();
}

}   // namespace mc

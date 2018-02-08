/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Application.hpp"

namespace mc {

static bool LoadGlad();

Application::Application(unsigned int winX, unsigned int winY):
    active { true },
    window { sf::VideoMode(winX, winY),
            "MC",
            sf::Style::Default,
            sf::ContextSettings(24, 8, 4, 4, 6) },
    world { nullptr } {

    LoadGlad();

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    world = std::make_unique<World>();
}

Application::~Application() {

}

void Application::Loop() {
    INFO("Starting application main loop");
    sf::Clock clock;

    while (active) {
        HandleMouse();
        HandleEvents();
        DrawScene();
        sf::sleep(sf::milliseconds(20));
        delta = clock.restart();
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

void Application::HandleMouse() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();
    float winX = windowSize.x / 2;
    float winY = windowSize.y / 2;
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), window);

    float d = delta.asMilliseconds() / 1000.0f;
    float horizontal = 0.025 * d * float(winX - mousePos.x);
    float vertical =  0.025 * d * float(winY - mousePos.y);

    if (abs(horizontal) > std::numeric_limits<float>::epsilon() ||
        abs(vertical) > std::numeric_limits<float>::epsilon()) {
        Rotation offset = Rotation { -vertical, -horizontal, 0.0f };    //TODO fix negative angles
        world->GetCamera().Rotate(offset);
    }

}

void Application::DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world->Draw();
    window.display();
}

static bool LoadGlad() {
    if (!gladLoadGL()) {
        throw OpenGLError("gladLoadGL", __FUNCTION__);
    }
    INFO("Glad loaded");
    return true;
}


}   // namespace mc

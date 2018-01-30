/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Application.hpp"

namespace mc {

Application::Application(unsigned int winX, unsigned int winY):
    active { true },
    window { sf::VideoMode(winX, winY),
            "MC",
            sf::Style::Default,
            sf::ContextSettings(24, 8, 4, 4, 6) },
    shader { nullptr },
    mesh { nullptr } {

    if (!gladLoadGL()) {
        throw OpenGLError("gladLoadGL", __FUNCTION__);
    }

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    const GLfloat vertices[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    shader = std::make_unique<ShaderProgram>();
    shader->AddVertexShader("shader/VertexShader.glsl");
    shader->AddFragmentShader("shader/FragmentShader.glsl");
    shader->Link();
    shader->Use();

    camera = std::make_unique<Camera>();

    mesh = std::make_unique<Mesh>(vertices, 36);

}

Application::~Application() {
    log::DestroyGlobalLogger();
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
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2));

    float d = delta.asMilliseconds() / 1000.0f;
    float horizontalAngle = 0.3 * d * float(windowSize.x / 2 - mousePos.x);
    float verticalAngle = 0.3 * d * float(windowSize.y / 2 - mousePos.y);

    camera->Rotate(glm::vec2(horizontalAngle, verticalAngle));

}

void Application::DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh->Draw(*camera, *shader);
    window.display();
}

}   // namespace mc

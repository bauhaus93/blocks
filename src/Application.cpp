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

    shader = std::make_unique<ShaderProgram>();
    shader->AddVertexShader("shader/VertexShader.glsl");
    shader->AddFragmentShader("shader/FragmentShader.glsl");
    shader->Link();
    shader->Use();

    mesh = std::make_unique<Mesh>("cube.obj");
    texture = std::make_unique<Texture>("test.bmp");
    sampleTexId = glGetUniformLocation(shader->GetId(), "myTexture");

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
    float horizontalAngle = 0.025 * d * float(winX - mousePos.x);
    float verticalAngle = 0.025 * d * float(winY - mousePos.y);

    if (abs(horizontalAngle) > std::numeric_limits<float>::epsilon() ||
        abs(verticalAngle) > std::numeric_limits<float>::epsilon()) {
        camera->Rotate(horizontalAngle, verticalAngle);
        //INFO("(", horizontalAngle, ", ", verticalAngle, ")");
    }

}

void Application::DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetId());
    glUniform1i(sampleTexId, 0);

    mesh->Draw(*camera, *shader);
    window.display();
}

}   // namespace mc

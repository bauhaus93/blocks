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

    constexpr GLfloat vertices[] = { -1.0f, -1.0f, 0.0f,
                            1.0f, -1.0f, 0.0f,
                            0.0f, 1.0f, 0.0f };

    shader = std::make_unique<ShaderProgram>();
    shader->AddVertexShader("shader/VertexShader.glsl");
    shader->AddFragmentShader("shader/FragmentShader.glsl");
    shader->Link();
    shader->Use();

    camera = std::make_unique<Camera>();

    mesh = std::make_unique<Mesh>(vertices, 9);

}

Application::~Application() {
    log::DestroyGlobalLogger();
}

void Application::Loop() {
    INFO("Starting application main loop");


    while (active) {
        HandleEvents();
        DrawScene();
        sf::sleep(sf::milliseconds(20));
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
    mesh->Draw(*camera, *shader);
    window.display();
}

}   // namespace mc

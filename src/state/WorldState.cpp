/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "WorldState.hpp"

namespace mc {

WorldState::WorldState(sf::Window& window_):
    GameState(window_, 20),
    world { } {
}

State WorldState::GetState() const {
    return State::WORLD;
}

void WorldState::Tick() {
    HandleMouseMovement();
    HandleKeys();
    HandleEvents();
    world.Tick();
    DrawScene();
}

void WorldState::HandleEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        switch(event.type) {
        case sf::Event::Closed:
            SetResultAndLeave(true, State::EXIT);
            return;
        case sf::Event::Resized:
            glViewport(0, 0, event.size.width, event.size.height);
            break;
        case sf::Event::LostFocus:
            TRACE("Lost FOCUS");
            SetResultAndLeave(false, State::PAUSE);
            return;
        default:
            break;
        }
    }
}

void WorldState::HandleMouseMovement() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();
    float winX = static_cast<float>(windowSize.x) / 2.0f;
    float winY = static_cast<float>(windowSize.y) / 2.0f;
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), window);
    float diffX = float(winX - mousePos.x);
    float diffY = float(winY - mousePos.y);

    if (abs(diffX) > std::numeric_limits<float>::epsilon() ||
        abs(diffY) > std::numeric_limits<float>::epsilon()) {
        float d = lastDelta.asMilliseconds() / 1000.0f;
        diffX *= 0.025f * d;
        diffY *= 0.025f * d;
        Point3f offset(diffX, -diffY, 0.0f);
        world.GetCamera().Rotate(offset);
    }
}

void WorldState::HandleKeys() {
    constexpr float SPEED = 4.0f;
    glm::vec3 dir = CreateDirection(world.GetCamera().GetRotation());
    glm::vec3 totalOffset(0.0f, 0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        totalOffset += dir;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        totalOffset -= dir;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 0, 1)));
        totalOffset -= right;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 0, 1)));
        totalOffset += right;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 0, 1)));
        glm::vec3 up = glm::normalize(glm::cross(right, dir));
        totalOffset += up;
    }
    
    if (abs(totalOffset[0]) > 0.01f ||
        abs(totalOffset[1]) > 0.01f ||
        abs(totalOffset[2]) > 0.01f) {
            glm::vec3 normalized = glm::normalize(totalOffset);
            Point3f offset = Point3f(normalized[0], normalized[1], normalized[2]) * SPEED;
            world.GetCamera().Move(offset);
    }
}

void WorldState::DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.Draw();
    window.display();
}


}       // namespace mc

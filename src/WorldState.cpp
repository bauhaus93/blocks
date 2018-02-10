/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "WorldState.hpp"

namespace mc {

WorldState::WorldState(sf::Window& window_):
    GameState(window_, 20),
    world { } {

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
    float winX = windowSize.x / 2;
    float winY = windowSize.y / 2;
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), window);
    float diffX = float(winX - mousePos.x);
    float diffY = float(winY - mousePos.y);

    if (abs(diffX) > std::numeric_limits<float>::epsilon() ||
        abs(diffY) > std::numeric_limits<float>::epsilon()) {
        float d = lastDelta.asMilliseconds() / 1000.0f;
        diffX *= 0.15 * d;
        diffY *= 0.15 * d;
        Rotation offset = Rotation { diffX, -diffY, 0.0f };
        world.GetCamera().Rotate(offset);
    }
}

void WorldState::HandleKeys() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        auto dir = world.GetCamera().GetRotation().CreateDirection();
        Position offset = Position(dir[0], dir[1], dir[2]);
        world.GetCamera().Move(offset);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        auto dir = world.GetCamera().GetRotation().CreateDirection();
        Position offset = Position(-dir[0], -dir[1], -dir[2]);
        world.GetCamera().Move(offset);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {  //right is maybe left?!
        auto dir = world.GetCamera().GetRotation().CreateDirection();
        auto right = glm::normalize(glm::cross(glm::vec3(0, 0, 1), dir));
        Position offset = Position(right[0], right[1], right[2]);
        world.GetCamera().Move(offset);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        auto dir = world.GetCamera().GetRotation().CreateDirection();
        auto right = glm::normalize(glm::cross(glm::vec3(0, 0, 1), dir));
        Position offset = Position(-right[0], -right[1], -right[2]);
        world.GetCamera().Move(offset);
    }
}

void WorldState::DrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.Draw();
    window.display();
}


}       // namespace mc


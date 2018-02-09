/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "WorldState.hpp"

namespace mc {

WorldState::WorldState(std::vector<std::unique_ptr<GameState>>& stateStack_, sf::Window& window_):
    GameState(stateStack_),
    leave { false },
    window { window_ },
    world { } {

}

void WorldState::Run() {
    INFO("Entering WorldState");
    sf::Clock clock;

    leave = false;
    while (!leave) {    //TODO make comparsion with back of stack?!
        HandleMouseMovement();
        HandleKeys();
        HandleEvents();
        world.Tick();
        DrawScene();
        sf::sleep(sf::milliseconds(20));
        lastDelta = clock.restart();
    }
    INFO("Leaving WorldState");
}

void WorldState::HandleEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            stateStack.pop_back();
            leave  = true;
        } else if (event.type == sf::Event::Resized) {
           glViewport(0, 0, event.size.width, event.size.height);
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


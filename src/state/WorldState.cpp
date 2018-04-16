/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "WorldState.hpp"

namespace mc {

WorldState::WorldState(sf::RenderWindow& window_):
    GameState(window_, 20),
    world { },
    font { } {
    if (!font.loadFromFile("data/Electrolize-Regular.ttf")) {
        throw ApplicationError("FontLoadError", __FUNCTION__, "Could not load font from file");
    }
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
    HandleMovementKeys();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        world.SetDrawDistance(world.GetChunkDrawDistance() + 5);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        int32_t drawDistance = std::max(0, world.GetChunkDrawDistance() - 5);
        world.SetDrawDistance(drawDistance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        world.GetCamera().ModFOV(5.0f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        world.GetCamera().ModFOV(-5.0f);    
    }
}

void WorldState::HandleMovementKeys() {
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
        //glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 0, 1)));
        //glm::vec3 up = glm::normalize(glm::cross(right, dir));
        totalOffset += glm::vec3(0.0f, 0.0f, 1.0f);
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

    glMatrixMode(GL_MODELVIEW); glPushMatrix();
    glMatrixMode(GL_PROJECTION); glPushMatrix();
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT   | GL_ENABLE_BIT  |
                 GL_TEXTURE_BIT      | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    sf::Text text;
    text.setFont(font);
    text.setString("SERS");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(10, 10);
    window.resetGLStates();
    window.draw(text);

    window.display();

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glPopAttrib();

}


}       // namespace mc

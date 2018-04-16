/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "graphics/glad/Glad.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "world/World.hpp"
#include "utility/Point3.hpp"

#include "GameState.hpp"

namespace mc {

class WorldState: public GameState {

 public:

    explicit            WorldState(sf::RenderWindow& window_);
                        ~WorldState() = default;
    void                Tick() override;
    State               GetState() const override;

 private:

    void                HandleEvents();
    void                HandleMouseMovement();
    void                HandleKeys();
    void                HandleMovementKeys();
    void                DrawScene();

    world::World        world;
    sf::Font            font;
};


}       // namespace mc

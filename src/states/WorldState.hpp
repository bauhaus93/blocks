/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include "world/World.hpp"
#include "world/Rotation.hpp"

#include "GameState.hpp"

namespace mc {

class WorldState: public GameState {

 public:

    explicit            WorldState(sf::Window& window_);
                        ~WorldState() = default;
    void                Tick() override;
    State               GetState() const override;

 private:

    void                HandleEvents();
    void                HandleMouseMovement();
    void                HandleKeys();
    void                DrawScene();

    world::World        world;
};


}       // namespace mc

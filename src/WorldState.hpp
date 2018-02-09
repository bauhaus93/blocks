/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/OpenGLError.hpp"
#include "world/World.hpp"
#include "world/Rotation.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"

namespace mc {

class WorldState: public GameState {

 public: 

                        WorldState(std::vector<std::unique_ptr<GameState>>& stateStack_, sf::Window& window_);
                        ~WorldState() = default;
    void                Run() override;

 private:

    void                HandleEvents();
    void                HandleMouseMovement();
    void                HandleKeys();
    void                DrawScene();

    bool                leave;
    sf::Window&         window;
    World               world;
    sf::Time            lastDelta;
};
 

}       // namespace mc

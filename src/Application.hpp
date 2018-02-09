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
#include "GameState.hpp"
#include "WorldState.hpp"

namespace mc {

class Application {
  public:

                    Application(unsigned int winX, unsigned int winY);
                    ~Application();

    void            Loop();
private:

    sf::Window                                  window;
    std::vector<std::unique_ptr<GameState>>     stateStack;


};

}   // namespace mc

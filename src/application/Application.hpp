/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <memory>

#include "glad/glad.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/OpenGLError.hpp"
#include "state/GameState.hpp"
#include "state/StateStack.hpp"

namespace mc {

class Application {
  public:

                    Application(unsigned int winX, unsigned int winY);
                    ~Application();

    void            Run();
private:

    sf::Window          window;
    StateStack          stateStack;
};

}   // namespace mc

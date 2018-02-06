/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>

#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/OpenGLError.hpp"
#include "world/World.hpp"

namespace mc {

class Application {
 private:

    bool            active;
    sf::Time        delta;
    sf::Window      window;

    std::unique_ptr<World>  world;


                    Application();
    void            HandleEvents();
    void            HandleMouse();
    void            DrawScene();

 public:

                    Application(unsigned int winX, unsigned int winY);
                    ~Application();

    void            Loop();
};

}   // namespace mc

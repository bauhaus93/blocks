/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once


#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "logger/GlobalLogger.hpp"
#include "OpenGLError.hpp"

namespace mc {

class Application {
 private:

    bool            active;
    sf::Window      window;
    GLuint          vao;



    void            HandleEvents();
    void            DrawScene();

 public:

                    Application(unsigned int winX, unsigned int winY);
                    ~Application();

    void            Loop();
};

}   // namespace mc

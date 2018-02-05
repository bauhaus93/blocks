/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>

#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/OpenGLError.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Mesh.hpp"

namespace mc {

class Application {
 private:

    bool            active;
    sf::Time        delta;
    sf::Window      window;
    std::unique_ptr<ShaderProgram>  shader;
    std::unique_ptr<Mesh>           mesh;
    std::unique_ptr<Texture>        texture;
    GLuint                          sampleTexId;



    void            HandleEvents();
    void            HandleMouse();
    void            DrawScene();

 public:

                    Application(unsigned int winX, unsigned int winY);
                    ~Application();

    void            Loop();
};

}   // namespace mc

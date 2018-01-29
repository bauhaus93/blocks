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
#include "graphics/Camera.hpp"

namespace mc {

class Application {
 private:

    bool            active;
    sf::Window      window;
    std::unique_ptr<ShaderProgram>  shader;
    std::unique_ptr<Camera>         camera;
    std::unique_ptr<Mesh>           mesh;



    void            HandleEvents();
    void            DrawScene();

 public:

                    Application(unsigned int winX, unsigned int winY);
                    ~Application();

    void            Loop();
};

}   // namespace mc

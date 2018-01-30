/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

namespace mc {

class Mesh {

 public:

                Mesh();
                ~Mesh();
    void        Draw(Camera& camera, ShaderProgram& shader);

 private:
    GLuint       vao;
    GLuint       vertexBuffer;
    GLuint       textureBuffer;
    unsigned int vertexCount;
    glm::mat4    model;

};

}   // namespace mc

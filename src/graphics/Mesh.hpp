/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "Camera.hpp"
#include "ShaderProgram.hpp"

namespace mc {

class Mesh {

 public:

                Mesh(const GLfloat* vertices, unsigned int vertexCount);
                ~Mesh();
    void        Draw(Camera& camera, ShaderProgram& shader);

 private:
    GLuint      vao;
    GLuint      vertexBuffer;
    glm::mat4   model;

};

}   // namespace mc

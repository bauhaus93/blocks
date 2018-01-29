/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>

namespace mc {

class Mesh {

 public:

                Mesh(const GLfloat* vertices, unsigned int vertexCount);
                ~Mesh();
    void        Draw();

 private:
    GLuint vao;
    GLuint vertexBuffer;

};

}   // namespace mc

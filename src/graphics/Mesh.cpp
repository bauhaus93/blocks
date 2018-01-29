/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"


namespace mc {


Mesh::Mesh(const GLfloat* vertices, unsigned int vertexCount) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
}

Mesh::~Mesh() {

}

void Mesh::Draw() {
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}



}   // namespace mc

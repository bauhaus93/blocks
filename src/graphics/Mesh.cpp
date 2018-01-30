/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"


namespace mc {


Mesh::Mesh(const GLfloat* vertices, unsigned int vertexCount_):
    vao { 0 },
    vertexBuffer { 0 },
    vertexCount { vertexCount_ },
    model { glm::mat4(1.0f) } {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
}

Mesh::~Mesh() {

}

void Mesh::Draw(Camera& camera, ShaderProgram& shader) {

    glm::mat4 mvp = camera.CreateMVPMatrix(model);
    shader.SetMVPMatrix(mvp);

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
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glDisableVertexAttribArray(0);
}



}   // namespace mc

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"

namespace mc {

std::vector<GLfloat> GetVertices(const std::string& data);
std::vector<GLfloat> GetUVs(const std::string& data);
std::vector<GLfloat> GetNormals(const std::string& data);

Mesh::Mesh(const std::string& filename):
    vao { 0 },
    vertexBuffer { 0 },
    uvBuffer { 0 },
    vertexCount { 0 },
    model { glm::mat4(1.0f) } {

    std::vector<GLfloat> vertices, uvs, normals;
    LoadMeshDataFromFile(filename, vertices, uvs, normals);

    vertexCount = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);

}

Mesh::~Mesh() {
    if (vertexBuffer != 0) {
        glDeleteBuffers(1, &vertexBuffer);
    }
    if (uvBuffer != 0) {
        glDeleteBuffers(1, &uvBuffer);
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
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

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}





}   // namespace mc

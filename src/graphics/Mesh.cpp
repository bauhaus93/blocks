/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"

namespace mc {

Mesh::Mesh(const std::string& filename):
    vao { 0 },
    vertexBuffer { 0 },
    uvBuffer { 0 },
    normalBuffer { 0 },
    indexBuffer { 0 },
    indexCount { 0 } {
    DEBUG("Creating mesh from file");

    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> uvs;
    std::vector<uint32_t> indices;
    LoadMeshDataFromFile(filename, vertices, uvs, normals, indices);

    indexCount = indices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        uvs.size() * sizeof(glm::vec2),
        uvs.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        normals.size() * sizeof(glm::vec3),
        normals.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(uint32_t), 
        indices.data(),
        GL_STATIC_DRAW
    );

    DEBUG("Created mesh");
}

Mesh::~Mesh() {
    DEBUG("Destroying mesh");
    if (indexBuffer != 0) {
        glDeleteBuffers(1, &indexBuffer);
    }
    if (normalBuffer != 0) {
        glDeleteBuffers(1, &normalBuffer);
    }
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

void Mesh::Load() const {

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

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

void Mesh::Unload() const {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Mesh::Draw() const {
    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        nullptr
    );
}





}   // namespace mc

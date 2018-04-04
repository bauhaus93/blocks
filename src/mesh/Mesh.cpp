/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"

namespace mc::mesh {

//TODO texture stuff
Mesh::Mesh(std::vector<Triangle> triangles_):
    triangles { triangles_ },
    vao { 0 },
    vertexBuffer { 0 },
    //uvBuffer { 0 },
    normalBuffer { 0 },
    indexBuffer { 0 },
    indexCount { 0 } {

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

    std::map<Vertex, uint32_t> indexedVertices;

    for (auto& triangle: triangles) {
        for (uint8_t i = 0; i < 3; i++) {
            const Vertex& vert = triangle.GetVertex(i);
            auto iter = indexedVertices.find(vert);
            if (iter == indexedVertices.end()) {
                vertices.push_back(vert.GetGlmPos());
                normals.push_back(vert.GetGlmNormal());
                uint32_t index = static_cast<uint32_t>(vertices.size() - 1);
                indices.push_back(index);
                indexedVertices.emplace(vert, index);
            } else {
                indices.push_back(iter->second);
            }
        }
    }
    indexCount = indices.size();
    LoadVBOs(vertices, normals, indices);
    LoadVAO();
}

Mesh::Mesh(Mesh&& other):
    triangles { std::move(other.triangles) },
    vao { other.vao },
    vertexBuffer { other.vertexBuffer },
    normalBuffer { other.normalBuffer },
    indexBuffer { other.indexBuffer },
    indexCount { other.indexCount } {
    other.vao = 0;
    other.vertexBuffer = 0;
    other.normalBuffer = 0;
    other.indexBuffer = 0;
}

Mesh::~Mesh() {
    if (indexBuffer != 0) {
        glDeleteBuffers(1, &indexBuffer);
    }
    if (normalBuffer != 0) {
        glDeleteBuffers(1, &normalBuffer);
    }
    if (vertexBuffer != 0) {
        glDeleteBuffers(1, &vertexBuffer);
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

void Mesh::LoadVBOs(const std::vector<glm::vec3>& vertices,
                    const std::vector<glm::vec3>& normals,
                    const std::vector<uint32_t>& indices) {
    assert(vertexBuffer == 0);  //for now
    assert(normalBuffer == 0);  //will check later, what is needed
 
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
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
}

void Mesh::LoadVAO() {
    assert(vertexBuffer != 0);
    assert(normalBuffer != 0);
    assert(indexBuffer != 0);
    assert(vao == 0);   //will make recreation later

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);
}

void Mesh::Draw() const {
    glBindVertexArray(vao);
    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        nullptr
    );
    glBindVertexArray(0);
}

}       // namespace mc::mesh

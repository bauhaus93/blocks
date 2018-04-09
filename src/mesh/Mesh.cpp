/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"

namespace mc::mesh {

//TODO texture stuff
Mesh::Mesh(std::vector<Triangle> triangles_):
    triangles { std::move(triangles_) },
    vao { 0 },
    vertexBuffer { 0 },
    uvBuffer { 0 },
    normalBuffer { 0 },
    indexBuffer { 0 },
    indexCount { 0 },
    unsavedData { std::make_unique<VBOData>() } {

    std::map<Vertex, uint32_t> indexedVertices;

    for (auto& triangle: triangles) {
        for (uint8_t i = 0; i < 3; i++) {
            const Vertex& vert = triangle.GetVertex(i);
            auto iter = indexedVertices.find(vert);
            if (iter == indexedVertices.end()) {
                unsavedData->vertices.push_back(vert.GetGlmPos());
                unsavedData->uvs.push_back(vert.GetGlmUV());
                unsavedData->normals.push_back(vert.GetGlmNormal());
                uint32_t index = static_cast<uint32_t>(unsavedData->vertices.size() - 1);
                unsavedData->indices.push_back(index);
                indexedVertices.emplace(vert, index);
            } else {
                unsavedData->indices.push_back(iter->second);
            }
        }
    }
    indexCount = unsavedData->indices.size();
}

Mesh::Mesh(Mesh&& other):
    triangles { std::move(other.triangles) },
    vao { other.vao },
    vertexBuffer { other.vertexBuffer },
    uvBuffer { other.uvBuffer },
    normalBuffer { other.normalBuffer },
    indexBuffer { other.indexBuffer },
    indexCount { other.indexCount },
    unsavedData { std::move(other.unsavedData) } {
    other.vao = 0;
    other.vertexBuffer = 0;
    other.normalBuffer = 0;
    other.indexBuffer = 0;
}

Mesh& Mesh::operator=(Mesh&& other) {
    assert(vao == 0);
    assert(vertexBuffer == 0);
    assert(uvBuffer == 0);
    assert(normalBuffer == 0);
    assert(indexBuffer == 0);
    triangles = std::move(other.triangles);
    vao = other.vao;
    vertexBuffer = other.vertexBuffer;
    uvBuffer = other.uvBuffer;
    normalBuffer = other.normalBuffer;
    indexBuffer = other.indexBuffer;
    indexCount = other.indexCount;
    unsavedData = std::move(other.unsavedData);
    other.vao = 0;
    other.vertexBuffer = 0;
    other.uvBuffer = 0;
    other.normalBuffer = 0;
    other.indexBuffer = 0;
    other.unsavedData = nullptr;
    assert(vao != 0);
    assert(vertexBuffer != 0);
    assert(uvBuffer != 0);
    assert(normalBuffer != 0);
    assert(indexBuffer != 0);
    return *this; 
}

Mesh::~Mesh() {
    if (indexBuffer != 0) {
        glDeleteBuffers(1, &indexBuffer);
    }
    if (normalBuffer != 0) {
        glDeleteBuffers(1, &normalBuffer);
    }
    if (uvBuffer != 0) {
        glDeleteBuffers(1, &uvBuffer);
    }
    if (vertexBuffer != 0) {
        glDeleteBuffers(1, &vertexBuffer);
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

void Mesh::LoadVBOs() {
    assert(unsavedData != nullptr);
    assert(vertexBuffer == 0);
    assert(uvBuffer == 0);
    assert(normalBuffer == 0);
    assert(indexBuffer == 0);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        unsavedData->vertices.size() * sizeof(glm::vec3),
        unsavedData->vertices.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        unsavedData->uvs.size() * sizeof(glm::vec2),
        unsavedData->uvs.data(),
        GL_STATIC_DRAW
    );  

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        unsavedData->normals.size() * sizeof(glm::vec3),
        unsavedData->normals.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        unsavedData->indices.size() * sizeof(uint32_t), 
        unsavedData->indices.data(),
        GL_STATIC_DRAW
    );
    unsavedData = nullptr;
}

void Mesh::LoadVAO() {
    assert(vertexBuffer != 0);
    assert(uvBuffer != 0);
    assert(normalBuffer != 0);
    assert(indexBuffer != 0);
    assert(vao == 0);

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

    glBindVertexArray(0);
}

void Mesh::Draw() {
    if (vao == 0) {
        LoadVBOs();
        LoadVAO();
    }
    assert(vao != 0);
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


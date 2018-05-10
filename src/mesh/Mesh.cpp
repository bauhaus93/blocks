/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Mesh.hpp"

namespace mc::mesh {

static std::vector<Triangle> CreateTriangles(const std::vector<Quad>& quads);

Mesh::Mesh(std::vector<Triangle> triangles_):
    triangles { std::move(triangles_) },
    vao { 0 },
    vertexBuffer { 0 },
    uvBuffer { 0 },
    normalBuffer { 0 },
    indexBuffer { 0 },
    indexCount { 0 },
    bufferData { nullptr } {

    triangles.shrink_to_fit();
    CreateBufferData();
}

Mesh::Mesh(std::vector<Quad> quads):
    Mesh(CreateTriangles(std::move(quads))) {
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

Mesh::Mesh(Mesh&& other):
    triangles { std::move(other.triangles) },
    vao { other.vao },
    vertexBuffer { other.vertexBuffer },
    uvBuffer { other.uvBuffer },
    normalBuffer { other.normalBuffer },
    indexBuffer { other.indexBuffer },
    indexCount { other.indexCount },
    bufferData { std::move(other.bufferData) } {
    other.vao = 0;
    other.vertexBuffer = 0;
    other.normalBuffer = 0;
    other.indexBuffer = 0;
}

Mesh& Mesh::operator=(Mesh&& other) {
    if(this != &other) {
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
        bufferData = std::move(other.bufferData);
        other.vao = 0;
        other.vertexBuffer = 0;
        other.uvBuffer = 0;
        other.normalBuffer = 0;
        other.indexBuffer = 0;
        other.bufferData = nullptr;
        assert(vao != 0);
        assert(vertexBuffer != 0);
        assert(uvBuffer != 0);
        assert(normalBuffer != 0);
        assert(indexBuffer != 0);
    }
    return *this;
}

void Mesh::AddQuads(const std::vector<Quad>& quads) {
    triangles.insert(triangles.end(), )
}

void Mesh::CreateBufferData() {
    bufferData = std::make_unique<VBOData>();
    std::map<Vertex, uint32_t> indexedVertices;

    for (auto& triangle: triangles) {
        for (uint8_t i = 0; i < 3; i++) {
            const Vertex& vert = triangle.GetVertex(i);
            auto iter = indexedVertices.find(vert);
            if (iter == indexedVertices.end()) {
                bufferData->vertices.push_back(vert.GetGlmPos());
                bufferData->uvs.push_back(vert.GetGlmUV());
                bufferData->normals.push_back(vert.GetGlmNormal());
                uint32_t index = static_cast<uint32_t>(bufferData->vertices.size() - 1);
                bufferData->indices.push_back(index);
                indexedVertices.emplace(vert, index);
            } else {
                bufferData->indices.push_back(iter->second);
            }
        }
    }
    indexCount = bufferData->indices.size();
}


void Mesh::LoadVBOs() {
    assert(bufferData != nullptr);
    assert(vertexBuffer == 0);
    assert(uvBuffer == 0);
    assert(normalBuffer == 0);
    assert(indexBuffer == 0);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        bufferData->vertices.size() * sizeof(glm::vec3),
        bufferData->vertices.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        bufferData->uvs.size() * sizeof(glm::vec3),
        bufferData->uvs.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        bufferData->normals.size() * sizeof(glm::vec3),
        bufferData->normals.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        bufferData->indices.size() * sizeof(uint32_t),
        bufferData->indices.data(),
        GL_STATIC_DRAW
    );
    bufferData = nullptr;
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
        3,
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
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
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

static std::vector<Triangle> CreateTriangles(const std::vector<Quad>& quads) {
    std::vector<Triangle> triangles;
    triangles.reserve(quads.size() * 2);

    for (auto& quad: quads) {
        triangles.emplace_back(quad.GetFirstTriangle());
        triangles.emplace_back(quad.GetSecondTriangle());
    }
    return triangles;
}


}       // namespace mc::mesh

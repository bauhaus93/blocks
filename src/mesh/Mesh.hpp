/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <map>
#include <memory>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "logger/GlobalLogger.hpp"
#include "glad/glad.h"
#include "utility/Point3.hpp"

#include "Vertex.hpp"
#include "Quad.hpp"
#include "Triangle.hpp"

namespace mc::mesh {

struct VBOData {
    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  uvs;
    std::vector<glm::vec3>  normals;
    std::vector<uint32_t>   indices;
};

class Mesh {
 public:
    explicit                        Mesh(std::vector<Triangle> triangles_);
    explicit                        Mesh(std::vector<Quad> quads);
                                    ~Mesh();
                                    Mesh(Mesh&& other);
    Mesh&                           operator=(Mesh&& other);

    std::size_t                     GetTriangleCount() const { return triangles.size(); }
    const std::vector<Triangle>&    GetTriangles() const { return triangles; }
    bool                            IsEmpty() const { return triangles.empty(); }
    void                            Draw();

 private:

    void        CreateBufferData();
    void        LoadVBOs();
    void        LoadVAO();

    std::vector<Triangle> triangles;

    GLuint      vao;
    GLuint      vertexBuffer;
    GLuint      uvBuffer;
    GLuint      normalBuffer;
    GLuint      indexBuffer;
    uint32_t    indexCount;

    std::unique_ptr<VBOData>        bufferData;
};

}       // namespace mc::mesh

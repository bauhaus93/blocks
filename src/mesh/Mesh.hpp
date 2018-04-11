/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <vector>
#include <map>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/glad/Glad.h"
#include "utility/Point3.hpp"

#include "Vertex.hpp"
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
    explicit    Mesh(std::vector<Triangle> triangles_);
                ~Mesh();
                Mesh(Mesh&& other);
    Mesh&       operator=(Mesh&& other);

    std::size_t GetTriangleCount() const { return triangles.size(); }
    void        Draw();

 private:

    void        LoadVBOs();
    void        LoadVAO();

    std::vector<Triangle> triangles;

    GLuint      vao;
    GLuint      vertexBuffer;
    GLuint      uvBuffer;
    GLuint      normalBuffer;
    GLuint      indexBuffer;
    uint32_t    indexCount;

    std::unique_ptr<VBOData>    unsavedData;
};

}       // namespace mc::mesh

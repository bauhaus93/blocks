/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <vector>
#include <map>
#include <memory>

#include <glm/vec3.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/glad/Glad.h"
#include "utility/Point3.hpp"

#include "Vertex.hpp"
#include "Triangle.hpp"

namespace mc::mesh {

class Mesh {
 public:
    explicit    Mesh(std::vector<Triangle> triangles_);
                Mesh(Mesh&& other);
    Mesh&       operator=(Mesh&& other);
                ~Mesh();
    void        Draw() const;

 private:

    void        LoadVBOs(const std::vector<glm::vec3>& vertices,
                         const std::vector<glm::vec3>& normals,
                         const std::vector<uint32_t>& indices);
    void        LoadVAO();

    std::vector<Triangle> triangles;

    GLuint      vao;
    GLuint      vertexBuffer;
    //GLuint      uvBuffer;
    GLuint      normalBuffer;
    GLuint      indexBuffer;
    uint32_t    indexCount;
};

}       // namespace mc::mesh

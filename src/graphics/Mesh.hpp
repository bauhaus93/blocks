/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "utility/FileRead.hpp"
#include "world/Camera.hpp"

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "MeshLoader.hpp"

namespace mc {

class Mesh {

 public:

    explicit     Mesh(const std::string& filename);
                ~Mesh();
    void        Draw() const;

 private:
    GLuint       vao;
    GLuint       vertexBuffer;
    GLuint       uvBuffer;
    unsigned int vertexCount;
    glm::mat4    model;

};

}   // namespace mc

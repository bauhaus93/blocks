/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "glad/Glad.h"

#include "MeshLoader.hpp"

namespace mc {

class Mesh {

 public:

    explicit     Mesh(const std::string& filename);
                ~Mesh();
    void        Load() const;
    void        Unload() const;
    void        Draw() const;

 private:
    GLuint       vao;
    GLuint       vertexBuffer;
    GLuint       uvBuffer;
    GLuint       normalBuffer;
    GLuint       indexBuffer;
    std::size_t	 indexCount;
};

}   // namespace mc

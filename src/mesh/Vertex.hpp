/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "utility/Point3.hpp"
#include "utility/Volume.hpp"

namespace mc::mesh {

class Vertex {
 public:
                Vertex(Point3f pos_, Point3f uv_, Point3f normal_);

    bool        operator<(const Vertex& other) const;
    Point3f     GetPos() const { return pos; }
    Point3f     GetUV() const { return uv; }
    Point3f     GetNormal() const { return normal; }
    glm::vec3   GetGlmPos() const;
    glm::vec3   GetGlmUV() const;
    glm::vec3   GetGlmNormal() const;
    bool        InVolume(const Volume& volume) const;

 private:
    Point3f pos;
    Point3f uv;
    Point3f normal;
};

std::ostream& operator<<(std::ostream& os, const Vertex& vertex);

}       // namespace mc::mesh

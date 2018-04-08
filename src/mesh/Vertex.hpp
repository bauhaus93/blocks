/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "utility/Point2.hpp"
#include "utility/Point3.hpp"

namespace mc::mesh {

class Vertex {
 public:
                Vertex(Point3f pos_, Point2f uv_, Point3f normal_);

    bool        operator<(const Vertex& other) const;
    glm::vec3   GetGlmPos() const;
    glm::vec2   GetGlmUV() const;
    glm::vec3   GetGlmNormal() const;

 private:
    Point3f pos;
    Point2f uv;
    Point3f normal;
};

}       // namespace mc::mesh




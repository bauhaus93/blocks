/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Triangle.hpp"

namespace mc::mesh {

Triangle::Triangle():
    vertex { { Vertex { Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f) } } } {
}

void Triangle::SetVertex(uint8_t index, Vertex vert) {
    assert(index < 3);
    vertex[index] = vert;
}

const Vertex& Triangle::GetVertex(uint8_t index) const {
    assert(index < 3);
    return vertex[index];
}

}       // namespace mc::mesh

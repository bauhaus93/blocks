/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Quad.hpp"

namespace blocks {

Quad::Quad():
    vertex { { Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) } } }  {
}

void Quad::SetVertex(uint8_t index, Vertex vert) {
    assert(index < 4);
    vertex[index] = vert;
}

const Vertex& Quad::GetVertex(uint8_t index) const {
    assert(index < 4);
    return vertex[index];
}

Triangle Quad::GetFirstTriangle() const {
    Triangle t;
    t.SetVertex(0, vertex[0]);
    t.SetVertex(1, vertex[1]);
    t.SetVertex(2, vertex[2]);
    return t;
}

Triangle Quad::GetSecondTriangle() const {
    Triangle t;
    t.SetVertex(0, vertex[2]);
    t.SetVertex(1, vertex[3]);
    t.SetVertex(2, vertex[0]);
    return t;
}

std::ostream& operator<<(std::ostream& os, const Quad& quad) {
    os << "Quad: ";
    for (uint8_t i = 0; i < 4; i++) {
        os << quad.GetVertex(i);
        if (i != 3) {
            os << ", ";
        }
    }
    return os;
}

}       // namespace blocks

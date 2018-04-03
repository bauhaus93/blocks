/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Quad.hpp"

namespace mc::mesh {

Quad::Quad():
    vertex { { Vertex { Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f) } } }  {

}

void Quad::SetVertex(uint8_t index, Vertex vert) {
    assert(index < 4);
    vertex[index] = vert;
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


}       // namespace mc::mesh

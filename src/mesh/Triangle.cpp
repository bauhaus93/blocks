/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Triangle.hpp"

namespace blocks {

Triangle::Triangle():
    vertex { { Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) },
               Vertex { Point3f(0.0f), Point3f(0.0f), Point3f(0.0f) } } } {
}

void Triangle::SetVertex(uint8_t index, Vertex vert) {
    assert(index < 3);
    vertex[index] = vert;
}

const Vertex& Triangle::GetVertex(uint8_t index) const {
    assert(index < 3);
    return vertex[index];
}

/*std::pair<Triangle, Triangle> Triangle::Subdivide() const {
    std::pair<Triangle, Triangle> triangles;
    float maxDist = 0.0f;
    uint8_t hypIndex = 0;
    for (uint8_t i = 0; i < 3; i++) {
        float dist = vertex[i].GetDistance(vertex[(i + 1) % 3]);
        if (dist > maxDist) {
            maxDist = dist;
            hypIndex = i;
        }
    }
    Point3f dirVec = (vertex[(hypIndex + 1) % 3].GetPos() - vertex[hypIndex].GetPos()) / 2.0f;
    Point3f newPos = vertex[hypIndex].GetPos() + dirVec;
    Point3f newUV = vertex[hypIndex].GetUV() + dirVec;
    return triangles;
}*/

bool Triangle::InVolume(const Volume& volume) const {
    for (auto& v: vertex) {
        if (!v.InVolume(volume)) {
            return false;
        }
    }
    return true;
}

}       // namespace blocks

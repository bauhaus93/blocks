/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>

#include "Vertex.hpp"
#include "Triangle.hpp"

namespace mc::mesh {

class Quad {
 public:
            Quad();
    void    SetVertex(uint8_t index, Vertex vert);
    Triangle GetFirstTriangle() const;
    Triangle GetSecondTriangle() const;
 private:
    std::array<Vertex, 4> vertex;
};

}       // namespace mc::mesh


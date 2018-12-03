/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <array>
#include <memory>

#include "Vertex.hpp"
#include "Triangle.hpp"

namespace blocks {

class Quad {
 public:
            Quad();
            Quad(Quad&& other) = default;
    Quad&   operator=(Quad&& other) = default;
    void    SetVertex(uint8_t index, Vertex vert);
    const Vertex& GetVertex(uint8_t index) const; 
    Triangle GetFirstTriangle() const;
    Triangle GetSecondTriangle() const;
 private:
    std::array<Vertex, 4> vertex;
};

std::ostream& operator<<(std::ostream& os, const Quad& quad);

}       // namespace blocks


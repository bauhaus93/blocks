/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>

#include "Vertex.hpp"

namespace mc::mesh {

class Triangle {
 public:
                Triangle();
    void        SetVertex(uint8_t index, Vertex vert);
    const Vertex& GetVertex(uint8_t index) const;
 private:
    std::array<Vertex, 3>  vertex;
};


}       // namespace mc::mesh


/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>
#include <memory>

#include "utility/Volume.hpp"
#include "Vertex.hpp"

namespace mc::mesh {

class Triangle {
 public:
                Triangle();
                Triangle(Triangle&& other) = default;
    Triangle&   operator=(Triangle&& other) = default;
    void        SetVertex(uint8_t index, Vertex vert);
    const Vertex& GetVertex(uint8_t index) const;
    std::pair<Triangle, Triangle> Subdivide() const;
    bool        InVolume(const Volume& volume) const;

 private:
    std::array<Vertex, 3>  vertex;
};


}       // namespace mc::mesh

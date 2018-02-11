/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>
#include <cstdint>

#include "utility/Point3.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Mesh.hpp"
#include "Position.hpp"
#include "Cube.hpp"
#include "Camera.hpp"

namespace mc {

class Chunk {
 public:
            Chunk(const Position& origin_,
                  const Point3<float>& blockSize_,
                  const Point3<uint32_t>& gridSize_);

    void    Generate(const Mesh& mesh, const Texture& texture);
    void    Draw(const Camera& camera) const;
 private:
    
    const Position                      origin;
    const Point3<float>                 blockSize;
    const Point3<uint32_t>              gridSize;
    std::map<Point3<uint32_t>, Cube>    grid;
};

}       // namespace mc





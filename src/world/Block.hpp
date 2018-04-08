/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"

#include "Camera.hpp"
#include "NeighbourMask.hpp"
#include "Direction.hpp"

namespace mc::world {

class Block {

 public:
                Block(Point3i position_);
                Block(const Block& other);
    Point3i     GetPosition() const { return position; }
    void        AddNeighbour(Direction dir);
    void        RemoveNeighbour(Direction dir);
    bool        IsVisible() const { return !neighbours.IsFull(); }
    bool        HasNeighbour(Direction dir) const { return neighbours.Contains(dir); }
 private:
    Point3i             position;
    NeighbourMask       neighbours;
};



}   // namespace mc::world

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"
#include "world/Camera.hpp"
#include "world/NeighbourMask.hpp"
#include "world/Direction.hpp"
#include "BlockType.hpp"

namespace mc::world::chunk {

class Block {

 public:
                Block(Point3i position_, BlockType type_);
                Block(const Block& other);
    Point3i     GetPosition() const { return position; }
    BlockType   GetType() const { return type; }
    void        AddNeighbour(Direction dir);
    void        RemoveNeighbour(Direction dir);
    bool        IsVisible() const { return !neighbours.IsFull(); }
    bool        HasNeighbour(Direction dir) const { return neighbours.Contains(dir); }
 private:
    Point3i             position;
    BlockType           type;
    NeighbourMask       neighbours;
};



}   // namespace mc::world::chunk

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"
#include "world/Camera.hpp"
#include "world/NeighbourMask.hpp"
#include "world/Direction.hpp"
#include "world/ProtoBlock.hpp"

namespace mc::world::chunk {

class Block {
 public:
                        Block(Point3i8 position_, const ProtoBlock& prototype_);
    Point3i             GetPosition() const { return position; }
    const ProtoBlock&   GetPrototype() const { return protoype; }
    void                AddNeighbour(Direction dir);
    void                RemoveNeighbour(Direction dir);
    bool                IsVisible() const { return !neighbours.IsFull(); }
    bool                HasNeighbour(Direction dir) const { return neighbours.Contains(dir); }
 private:
    Point3i8            position;
    const ProtoBlock&   protoype;
    NeighbourMask       neighbours;
};



}   // namespace mc::world::chunk

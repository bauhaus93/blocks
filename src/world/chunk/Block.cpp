/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Block.hpp"

namespace mc::world::chunk {


Block::Block(Point3i position_, const ProtoBlock& prototype_):
    position { position_ },
    protoype { prototype_ },
    neighbours { } {
}

void Block::AddNeighbour(Direction dir) {
    neighbours.Add(dir);
}

void Block::RemoveNeighbour(Direction dir) {
    neighbours.Remove(dir);
}

}   // namespace mc::world::chunk

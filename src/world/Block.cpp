/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Block.hpp"

namespace mc::world {


Block::Block(Point3i position_):
    position { position_ },
    neighbours { } {
}

Block::Block(const Block& other):
    position { other.position },
    neighbours { other.neighbours } {
}

void Block::AddNeighbour(Direction dir) {
    neighbours.Add(dir);
}

void Block::RemoveNeighbour(Direction dir) {
    neighbours.Remove(dir);
}

}   // namespace mc::world

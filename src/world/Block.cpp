/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Block.hpp"

namespace mc::world {


Block::Block():
    neighbours { } {
}

Block::Block(const Block& other):
    neighbours { other.neighbours } {
}

void Block::AddNeighbour(Direction dir) {
    neighbours.Add(dir);
}

void Block::RemoveNeighbour(Direction dir) {
    neighbours.Remove(dir);
}

}   // namespace mc::world

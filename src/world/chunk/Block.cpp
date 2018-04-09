/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Block.hpp"

namespace mc::world::chunk {


Block::Block(Point3i position_, BlockType type_):
    position { position_ },
    type { type_ },
    neighbours { } {
}

Block::Block(const Block& other):
    position { other.position },
    type { other.type },
    neighbours { other.neighbours } {
}

void Block::AddNeighbour(Direction dir) {
    neighbours.Add(dir);
}

void Block::RemoveNeighbour(Direction dir) {
    neighbours.Remove(dir);
}

}   // namespace mc::world::chunk

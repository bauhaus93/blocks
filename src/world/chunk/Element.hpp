/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>

#include "Chunk.hpp"
#include "Direction.hpp"

namespace mc::world::chunk {

class Element {
public:
    explicit        Element(Chunk&& chunk_);
    void            Link(Element* neighbour, Direction direction);
    const Chunk&    GetChunk() const { return chunk; }
    Element*        GetNeighbour(Direction dir);

private:
    void            LinkElement(Element* neighbour, Direction direction);
    Chunk           chunk;
    Element*        neighbour[4];
};

}   // namespace mc::world::chunk

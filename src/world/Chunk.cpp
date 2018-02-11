/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"


namespace mc {


Chunk::Chunk(const Position& origin_, const Point3<uint32_t>& gridSize_):
    origin { origin_ },
    gridSize { gridSize_ },
    grid { } {
}



}       // namespace mc

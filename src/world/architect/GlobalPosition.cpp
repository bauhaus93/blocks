/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "GlobalPosition.hpp"

namespace mc::world::architect {

Point2i GetGlobalPosition(Point2i chunkPos, Point2i8 localPos) {
    return chunkPos * CHUNK_SIZE + localPos;
}

}   // namespace mc::world::architect
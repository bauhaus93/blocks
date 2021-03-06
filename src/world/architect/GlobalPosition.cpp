/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "GlobalPosition.hpp"

namespace blocks {

Point2i GetGlobalPosition(Point2i chunkPos, Point2i8 localPos) {
    return chunkPos * CHUNK_SIZE + localPos;
}

Point3i GetGlobalPosition(Point3i chunkPos, Point3i8 localPos) {
    return chunkPos * CHUNK_SIZE + localPos;
}

}   // namespace blocks

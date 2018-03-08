/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "utility/Point3.hpp"
#include "Chunk.hpp"

namespace mc::world::chunk {

struct PointChunkCmp {
    bool operator()(const Point3i& pos, std::pair<const Point3i, Chunk>& pair) const {
        return pos < pair.first;
    }

    bool operator()(std::pair<const Point3i, Chunk>& pair, const Point3i& pos) const {
        return pair.first < pos;
    }

    bool operator()(const Point3i& pos, const Chunk& chunk) const {
        return pos < chunk.GetPosition();
    }

    bool operator()(const Chunk& chunk, const Point3i& pos) const {
        return chunk.GetPosition() < pos;
    }
};

}   // namespace mc::world::chunk
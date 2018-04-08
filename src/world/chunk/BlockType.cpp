/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "BlockType.hpp"

namespace mc::world::chunk {

static Point2i GetGrassAtlasField(Direction dir);
static Point2i GetMudAtlasField(Direction dir);

Point2i GetAtlasField(BlockType type, Direction dir) {
    switch (type) {
        case BlockType::GRASS:  return GetGrassAtlasField(dir);
        case BlockType::MUD:    return GetMudAtlasField(dir);
        default: assert(0);
    }
    return Point2i(0, 0);
}

static Point2i GetGrassAtlasField(Direction dir) {
    switch(dir) {
        case Direction::UP: return Point2i(1, 0);
        default:            return Point2i(0, 0);
    }
    return Point2i(0, 0);
}

static Point2i GetMudAtlasField(Direction dir) {
    return Point2i(0, 0);
}

}   // namespace mc::world::chunk

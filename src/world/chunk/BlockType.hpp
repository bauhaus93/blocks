/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "utility/Point2.hpp"
#include "world/Direction.hpp"

namespace mc::world::chunk {

enum class BlockType {
    GRASS,
    MUD
};

Point2i GetAtlasField(BlockType type, Direction dir);

}   // namespace mc::world::chunk

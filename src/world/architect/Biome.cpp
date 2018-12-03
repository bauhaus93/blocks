/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Biome.hpp"

namespace blocks {

Biome::Biome():
    blockType { BlockType::MUD } {
}

void Biome::SetBlockType(BlockType type) {
    blockType = type;
}

}   // namespace blocks
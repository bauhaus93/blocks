/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Biome.hpp"

namespace mc::world::architect {

Biome::Biome():
    blockType { BlockType::MUD } {
}

void Biome::SetBlockType(BlockType type) {
    blockType = type;
}

}   // namespace mc::world::architect
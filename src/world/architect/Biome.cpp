/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Biome.hpp"

namespace mc::world::architect {

Biome::Biome(uint32_t seed):
    biomeNoise { seed },
    heightNoise { seed + 1 },
    blockType { BlockType::MUD } {
}

Biome::Biome(Biome&& other):
    biomeNoise { std::move(other.biomeNoise) },
    heightNoise { std::move(other.heightNoise) },
    blockType { other.blockType } {
}

Biome& Biome::operator=(Biome&& other) {
    if (this != &other) {
        biomeNoise = std::move(other.biomeNoise);
        heightNoise = std::move(other.heightNoise);
        blockType = other.blockType;
    }
    return *this;
}

void Biome::SetBlockType(BlockType type) {
    blockType = type;
}

int32_t Biome::GetValue(Point2i globalPosition) const {
    return std::round(biomeNoise.GetNoise(globalPosition));
}

int32_t Biome::GetHeight(Point2i globalPosition) const {
    return std::round(heightNoise.GetNoise(globalPosition));
}

}   // namespace mc::world::architect
/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <random>
#include <memory>

#include "world/BlockType.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::architect {


enum class BiomeType {
    GRASSLANDS,
    MUDDY,
    DESERT,
    MOUNTAIN,
    HILLS
};

class Biome {
 public:
                            Biome();
    void                    SetBlockType(BlockType type);

    BlockType               GetBlockType() const { return blockType; }

    int32_t                 GetValue(Point2i globalPosition) const;

 private:
    BlockType               blockType;
};


}   // namespace mc::world::architect
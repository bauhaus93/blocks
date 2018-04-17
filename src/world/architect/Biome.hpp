/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <random>
#include <memory>

#include "world/BlockType.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::architect {

class Biome {
 public:
                            Biome(uint32_t seed);
                            Biome(Biome&& other);
    Biome&                  operator=(Biome&& other);
    void                    SetMinHeight(int32_t minHeight_);
    void                    SetMaxHeight(int32_t maxHeight_);
    void                    SetBlockType(BlockType type);

    SimplexNoise&           GetBiomeNoise() { return biomeNoise; }
    SimplexNoise&           GetHeightNoise() { return heightNoise; }
    BlockType               GetBlockType() const { return blockType; }

    int32_t                 GetValue(Point2i globalPosition) const;
    int32_t                 GetHeight(Point2i globalPosition) const;

 private:
    SimplexNoise            biomeNoise;
    SimplexNoise            heightNoise;
    BlockType               blockType;
};


}   // namespace mc::world::architect
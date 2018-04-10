/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <random>

#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "BlockType.hpp"
#include "SimplexNoise.hpp"
#include "Size.hpp"

namespace mc::world {

class Architect {
 public:
                        Architect();
    explicit            Architect(uint32_t seed_);
    
    int32_t             GetAvgGlobalHeight(Point2i chunkPos) const;
    int32_t             GetGlobalHeight(Point2i globalPos) const;
    int32_t             GetGlobalHeight(Point2i chunkPos, Point2i localPos) const;
    int32_t             GetChunkRelativeHeight(Point3i chunkPos, Point2i localPos) const;
    BlockType           GetBlockType(Point3i chunkPos, Point3i localPos) const;

 private:
    const uint32_t      seed;
    std::mt19937        rng;
    const SimplexNoise  heightNoise;

};


}   // namespace mc::world
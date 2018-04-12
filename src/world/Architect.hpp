/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <random>
#include <limits>

#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "ProtoBlock.hpp"
#include "BlockType.hpp"
#include "SimplexNoise.hpp"
#include "Size.hpp"

namespace mc::world {

class Architect {
 public:
                        Architect(const std::map<BlockType, ProtoBlock>& protoblocks_);
    explicit            Architect(const std::map<BlockType, ProtoBlock>& protoblocks_, uint32_t seed_);
    
    std::pair<int32_t, int32_t> GetMinMaxGlobalHeight(Point2i chunkPos) const;
    int32_t             GetGlobalHeight(Point2i globalPos) const;
    int32_t             GetGlobalHeight(Point2i chunkPos, Point2i localPos) const;
    int32_t             GetChunkRelativeHeight(Point3i chunkPos, Point2i localPos) const;
    const ProtoBlock&   GetBlockPrototype(Point3i chunkPos, Point3i localPos) const;

 private:
    const std::map<BlockType, ProtoBlock>&  protoblocks;
    const uint32_t                          seed;
    std::mt19937                            rng;
    const SimplexNoise                      heightNoise;

    int32_t                                 GetRawGlobalHeight(Point2i globalPos) const;

};


}   // namespace mc::world
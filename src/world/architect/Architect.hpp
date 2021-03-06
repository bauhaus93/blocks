/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <random>
#include <limits>

#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "world/BlockType.hpp"
#include "world/Size.hpp"
#include "world/BlockManager.hpp"
#include "world/chunk/Chunk.hpp"
#include "world/chunk/Blocktree.hpp"
#include "SimplexNoise.hpp"
#include "GlobalPosition.hpp"
#include "Biome.hpp"

namespace blocks {

class Architect {
 public:
    explicit            Architect(const BlockManager& blockManager_);
                        Architect(const BlockManager& blockManager_, uint32_t seed_);

    Chunk        CreateChunk(const Point3i& chunkPos) const;

    BlockType           GetBlockType(Point3i chunkPos, Point3i8 localPos) const;

    std::pair<int32_t, int32_t> GetMinMaxGlobalHeight(Point2i chunkPos) const;
    int32_t             GetGlobalHeight(Point2i globalPos) const;
    int32_t             GetGlobalHeight(Point2i chunkPos, Point2i8 localPos) const;
    int32_t             GetChunkRelativeHeight(Point3i chunkPos, Point2i8 localPos) const;
    const BlockManager& GetBlockManager() const { return blockManager; }

 private:
    const BlockManager&                     blockManager;
    uint32_t                                seed;
    std::mt19937                            rng;
    SimplexNoise                            heightNoise;
    SimplexNoise                            temperatureNoise;
    SimplexNoise                            humidityNoise;
    SimplexNoise                            hillNoise;
    SimplexNoise                            mountainNoise;
    std::map<BiomeType, Biome>              biomes;

    void                                    LoadBiomes();
    int32_t                                 GetRawGlobalHeight(Point2i globalPos) const;
    const Biome&        GetBiome(Point2i chunkPos, Point2i8 localPos);
    const Biome&        GetBiome(Point2i globalPos) const;
};


}   // namespace blocks

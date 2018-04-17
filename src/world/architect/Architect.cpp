/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Architect.hpp"

namespace mc::world::architect {

Architect::Architect(const std::map<BlockType, ProtoBlock>& protoblocks_):
    Architect(protoblocks_, static_cast<uint32_t>(std::random_device{}())) {
}

Architect::Architect(const std::map<BlockType, ProtoBlock>& protoblocks_, uint32_t seed_):
    protoblocks { protoblocks_ },
    seed { seed_ },
    rng { seed } {
    LoadBiomes();
}

void Architect::LoadBiomes() {
    Biome grasslands(rng());
    grasslands.SetBlockType(BlockType::GRASS);

    grasslands.GetBiomeNoise().SetMin(0.0);
    grasslands.GetBiomeNoise().SetMax(100.0);
    grasslands.GetBiomeNoise().SetOctaves(6);
    grasslands.GetBiomeNoise().SetRoughness(0.5);
    grasslands.GetBiomeNoise().SetScale(0.0005);

    grasslands.GetHeightNoise().SetMin(100.0);
    grasslands.GetHeightNoise().SetMax(125.0);
    grasslands.GetHeightNoise().SetOctaves(6);
    grasslands.GetHeightNoise().SetRoughness(0.5);
    grasslands.GetHeightNoise().SetScale(0.0025);

    biomes.push_back(std::move(grasslands));

    Biome hills(rng());
    hills.SetBlockType(BlockType::GRASS);

    hills.GetBiomeNoise().SetMin(0.0);
    hills.GetBiomeNoise().SetMax(25.0);
    hills.GetBiomeNoise().SetOctaves(6);
    hills.GetBiomeNoise().SetRoughness(0.5);
    hills.GetBiomeNoise().SetScale(0.000075);

    hills.GetHeightNoise().SetMin(100.0);
    hills.GetHeightNoise().SetMax(200.0);
    hills.GetHeightNoise().SetOctaves(6);
    hills.GetHeightNoise().SetRoughness(0.5);
    hills.GetHeightNoise().SetScale(0.0025);

    biomes.push_back(std::move(hills));
}

const Biome& Architect::GetBiome(Point2i globalPos) const {
    double max = -1.0f;
    std::size_t maxPos = 0;
    for (std::size_t i = 0; i < biomes.size(); i++) {
        double value = biomes[i].GetValue(globalPos);
        if (value > max) {
            max = value;
            maxPos = i;
        }
    }
    return biomes[maxPos];
}

const Biome& Architect::GetBiome(Point2i chunkPos, Point2i localPos) {
    return GetBiome(GetGlobalPosition(chunkPos, localPos));
}

const ProtoBlock& Architect::GetProtoBlock(Point3i chunkPos, Point3i localPos) const {
    BlockType type = GetBiome(GetGlobalPosition(Point2i(chunkPos[0], chunkPos[1]),
                                                Point2i(localPos[0], localPos[1]))).GetBlockType();
    return protoblocks.at(type);
}

std::pair<int32_t, int32_t> Architect::GetMinMaxGlobalHeight(Point2i chunkPos) const {
    static const std::array<Point2i, 7> localPos { { Point2i(CHUNK_SIZE / 2),
                                                     Point2i(0),
                                                     Point2i(0, CHUNK_SIZE - 1),
                                                     Point2i(CHUNK_SIZE - 1, 0),
                                                     Point2i(CHUNK_SIZE - 1),
                                                     Point2i(CHUNK_SIZE / 2, 0),
                                                     Point2i(0, CHUNK_SIZE / 2) } };
    int32_t min = std::numeric_limits<int32_t>::max();
    int32_t max = 0;
    for (uint8_t i = 0; i < 7; i++) {
        int32_t h = GetRawGlobalHeight(GetGlobalPosition(chunkPos, localPos[i]));
        min = std::min(min, h);
        max = std::max(max, h);
    }
    return std::make_pair(min, max); 
}


int32_t Architect::GetChunkRelativeHeight(Point3i chunkPos, Point2i localPos) const {
    Point2i globalPos = GetGlobalPosition(Point2i(chunkPos[0], chunkPos[1]), localPos);
    return std::min(CHUNK_SIZE - 1, GetGlobalHeight(globalPos) - chunkPos[2] * CHUNK_SIZE - 1);
}

int32_t Architect::GetGlobalHeight(Point2i globalPos) const {
    constexpr int32_t range = 8;
    int32_t sum = 0;
    int32_t count = 0;
    for (int32_t y = -range; y <= range; y += 2) {
        Point2i off(0, y);
        sum += GetRawGlobalHeight(globalPos + off);
        count++;
    }
    for (int32_t x = -range; x < range; x += 2) {
        Point2i off(x, 0);
        sum += GetRawGlobalHeight(globalPos + off);
        count++;
    }
    return sum / count;
}

//solid hilly: 1.0/10.0/6/0.01/0.025
//really nice hilly: 1.0/100.0/6/0.5/0.0025
// smaller SCALE -> bigger results
int32_t Architect::GetRawGlobalHeight(Point2i globalPos) const {
    return GetBiome(globalPos).GetHeight(globalPos);
}

int32_t Architect::GetGlobalHeight(Point2i chunkPos, Point2i localPos) const {
    return GetGlobalHeight(GetGlobalPosition(chunkPos, localPos));
}


}   // namespace mc::world::architect
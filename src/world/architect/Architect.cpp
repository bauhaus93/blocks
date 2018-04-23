/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Architect.hpp"

namespace mc::world::architect {

Architect::Architect(const std::map<BlockType, ProtoBlock>& protoblocks_):
    Architect(protoblocks_, static_cast<uint32_t>(std::random_device{}())) {
}

Architect::Architect(const std::map<BlockType, ProtoBlock>& protoblocks_, uint32_t seed_):
    protoblocks { protoblocks_ },
    seed { seed_ },
    rng { seed },
    heightNoise { static_cast<uint32_t>(rng()) },
    temperatureNoise { static_cast<uint32_t>(rng()) },
    humidityNoise { static_cast<uint32_t>(rng()) },
    hillNoise { static_cast<uint32_t>(rng()) },
    mountainNoise { static_cast<uint32_t>(rng()) } {

    heightNoise.SetMin(0.0);
    heightNoise.SetMax(25.0);
    heightNoise.SetOctaves(5);
    heightNoise.SetRoughness(0.5);
    heightNoise.SetScale(0.003);

    temperatureNoise.SetMin(-20.0);
    temperatureNoise.SetMax(40.0);
    temperatureNoise.SetOctaves(4);
    temperatureNoise.SetRoughness(0);
    temperatureNoise.SetScale(0.0004);

    humidityNoise.SetMin(0.0);
    humidityNoise.SetMax(100.0);
    humidityNoise.SetOctaves(6);
    humidityNoise.SetRoughness(0.2);
    humidityNoise.SetScale(0.001);

    hillNoise.SetMin(-1.0);
    hillNoise.SetMax(1.0);
    hillNoise.SetOctaves(2);
    hillNoise.SetRoughness(7.0);
    hillNoise.SetScale(0.0004);

    mountainNoise.SetMin(-2.0);
    mountainNoise.SetMax(1.0);
    mountainNoise.SetOctaves(10);
    mountainNoise.SetRoughness(6.0);
    mountainNoise.SetScale(0.000003);

    LoadBiomes();
}

void Architect::LoadBiomes() {
    Biome grasslands;
    grasslands.SetBlockType(BlockType::GRASS);
    biomes.emplace(BiomeType::GRASSLANDS, grasslands);

    Biome mud;
    mud.SetBlockType(BlockType::MUD);
    biomes.emplace(BiomeType::MUDDY, mud);

    Biome desert;
    desert.SetBlockType(BlockType::DESERT);
    biomes.emplace(BiomeType::DESERT, desert);

    Biome mountain;
    mountain.SetBlockType(BlockType::ROCK);
    biomes.emplace(BiomeType::MOUNTAIN, mountain);

    Biome hills;
    hills.SetBlockType(BlockType::MUD);
    biomes.emplace(BiomeType::HILLS, hills);
}

const Biome& Architect::GetBiome(Point2i globalPos) const {
    //double height = heightNoise.GetNoise(globalPos);
    double temperature = temperatureNoise.GetNoise(globalPos);
    //double humidity = humidityNoise.GetNoise(globalPos);
    double hills = hillNoise.GetNoise(globalPos);
    double mountain = mountainNoise.GetNoise(globalPos);

    if (mountain > 0.0 && mountain > hills) {
        return biomes.at(BiomeType::MOUNTAIN);
    }
    if (hills > 0.0) {
        return biomes.at(BiomeType::HILLS);
    }
    if (temperature >= 30.0) {
        return biomes.at(BiomeType::DESERT);
    }
    return biomes.at(BiomeType::GRASSLANDS);
}

const Biome& Architect::GetBiome(Point2i chunkPos, Point2i8 localPos) {
    return GetBiome(GetGlobalPosition(chunkPos, localPos));
}

BlockType Architect::GetBlockType(Point3i chunkPos, Point3i8 localPos) const {
   return GetBiome(GetGlobalPosition(Point2i(chunkPos[0], chunkPos[1]),
                                     Point2i(localPos[0], localPos[1]))).GetBlockType();
}

std::pair<int32_t, int32_t> Architect::GetMinMaxGlobalHeight(Point2i chunkPos) const {
    static const std::array<Point2i, 7> localPos { { Point2i8(CHUNK_SIZE / 2),
                                                     Point2i8(0),
                                                     Point2i8(0, CHUNK_SIZE - 1),
                                                     Point2i8(CHUNK_SIZE - 1, 0),
                                                     Point2i8(CHUNK_SIZE - 1),
                                                     Point2i8(CHUNK_SIZE / 2, 0),
                                                     Point2i8(0, CHUNK_SIZE / 2) } };
    int32_t min = std::numeric_limits<int32_t>::max();
    int32_t max = 0;
    for (uint8_t i = 0; i < 7; i++) {
        int32_t h = GetGlobalHeight(GetGlobalPosition(chunkPos, localPos[i]));
        min = std::min(min, h);
        max = std::max(max, h);
    }
    return std::make_pair(min, max); 
}


int32_t Architect::GetChunkRelativeHeight(Point3i chunkPos, Point2i8 localPos) const {
    Point2i globalPos = GetGlobalPosition(Point2i(chunkPos[0], chunkPos[1]), localPos);
    return std::min(CHUNK_SIZE - 1, GetGlobalHeight(globalPos) - chunkPos[2] * CHUNK_SIZE - 1);
}

int32_t Architect::GetGlobalHeight(Point2i globalPos) const {
    return GetRawGlobalHeight(globalPos);
}

int32_t Architect::GetRawGlobalHeight(Point2i globalPos) const {
    double hill = hillNoise.GetNoise(globalPos);
    double mountain = mountainNoise.GetNoise(globalPos);
    double h = heightNoise.GetNoise(globalPos);
    double hHill = h, hMountain = h;

    if (hill > 0.0) {
        hHill *= 1.0 + (3.0 * hill);
        hHill = pow(1.0 - hill, 1.0) * h + pow(hill, 1.0) * hHill;
    }
    if (mountain > 0.0) {
        hMountain *= 1.0 + (25.0 * mountain);
        hMountain = pow((1.0 - mountain), 2.0) * h + pow(mountain, 2.0) * hMountain;
    }
    return std::max(std::max(hHill, hMountain), h);
}

int32_t Architect::GetGlobalHeight(Point2i chunkPos, Point2i8 localPos) const {
    return GetGlobalHeight(GetGlobalPosition(chunkPos, localPos));
}


}   // namespace mc::world::architect
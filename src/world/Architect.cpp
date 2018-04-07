/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Architect.hpp"

namespace mc::world {

Point2i GetGlobalPosition(Point2i chunkPos, Point2i localPos);

Architect::Architect():
    Architect(static_cast<uint32_t>(std::random_device{}())) {
}

Architect::Architect(uint32_t seed_):
    seed { seed_ },
    rng { seed },
    heightNoise { static_cast<uint32_t>(rng()) } {
}

int32_t Architect::GetChunkRelativeHeight(Point3i chunkPos, Point2i localPos) const {
    Point2i globalPos = GetGlobalPosition(Point2i(chunkPos[0], chunkPos[1]), localPos);
    return std::min(CHUNK_SIZE - 1, (GetGlobalHeight(globalPos) - chunkPos[2] * CHUNK_SIZE) - 1);

}

//solid hilly: 1.0/10.0/6/0.01/0.025
// smaller SCALE -> the bigger results
int32_t Architect::GetGlobalHeight(Point2i globalPos) const {
    constexpr double MIN_HEIGHT = 1.0;
    constexpr double MAX_HEIGHT = 100.0;
    constexpr double OCTAVES = 6;
    constexpr double ROUGHNESS = 0.5;
    constexpr double SCALE = 0.0025;
    double normalizedNoise = (1.0 + heightNoise.GetOctavedNoise(globalPos, OCTAVES, ROUGHNESS, SCALE)) / 2.0;
    return static_cast<int32_t>(MIN_HEIGHT + (MAX_HEIGHT - MIN_HEIGHT) * normalizedNoise);
}

int32_t Architect::GetGlobalHeight(Point2i chunkPos, Point2i localPos) const {
    return GetGlobalHeight(GetGlobalPosition(chunkPos, localPos));
}

Point2i GetGlobalPosition(Point2i chunkPos, Point2i localPos) {
    return chunkPos * CHUNK_SIZE + localPos;
}


}   // namespace mc::world
/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc {

World::World(Point2<int32_t> chunkSize_, Point3<float> blockSize_, int32_t activeChunkDistance_):
    camera { Position(4.0, 4.0, 0), Rotation(0, 0, 0) },
    noise { },
    mesh { "cube.obj" },
    texture { "test.bmp" },
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    centerChunkPos { GetChunkPos(camera) },
    activeChunkDistance { activeChunkDistance_ },
    activeChunks { } {
    INFO("Creating world");

    for (auto y = -activeChunkDistance; y < activeChunkDistance; y++) {
        for (auto x = -activeChunkDistance; x < activeChunkDistance; x++) {
            Point2<int32_t> chunkPosRel { x, y };
            Point2<int32_t> chunkPos { centerChunkPos + chunkPosRel };
            Chunk chunk { chunkPos, blockSize, chunkSize };
            chunk.Generate(noise, mesh, texture);
            activeChunks.emplace(chunkPosRel, std::move(chunk));
        }
    }
}

World::~World() {
    INFO("Destroying world");
}

void World::Tick() {
    Point2<int32_t> newCenterPos = GetChunkPos(camera);
    Point2<int32_t> diff = newCenterPos - centerChunkPos;

    if (diff[0] < 0) {    //new center left to old center
        for (auto y = -activeChunkDistance; y < activeChunkDistance; y++) {
            for (auto x = activeChunkDistance - 2 ; x > -activeChunkDistance; x--) {
                Point2<int32_t> oldRel { x, y };
                Point2<int32_t> newRel = oldRel + diff;
                Chunk chunk = std::move(activeChunks.at(oldRel));
                activeChunks.emplace(newRel, std::move(chunk));
            }
        }

    }

}

Point2<int32_t> World::GetChunkPos(const Entity& entity) {
    return Point2<int32_t>(static_cast<int32_t>(entity.GetPosition()[0] / chunkSize[0] / blockSize[0]),
                           static_cast<int32_t>(entity.GetPosition()[1] / chunkSize[1] / blockSize[1]));
}

void World::Draw() const {
    for (auto iter = activeChunks.begin(); iter != activeChunks.end(); ++iter) {
        iter->second.Draw(camera);
    }
}

}   // namespace mc

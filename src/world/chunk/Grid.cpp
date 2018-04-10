/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(int32_t chunkDrawDistance,
           const Architect& architect_):
    gridSize { Point3i(chunkDrawDistance) },
    refreshDistance { chunkDrawDistance / 10 },
    architect { architect_ },
    centerPos(1337, 1337, 1337),
    chunkLoader { 50, architect } {
    chunkLoader.Start();
}

Grid::~Grid() {
    chunkLoader.Stop();
}

void Grid::GivePositionUpdate(Point3f worldPos) {
    Point3i gridPos(worldPos / CHUNK_SIZE / BLOCK_SIZE);
    GivePositionUpdate(gridPos);
}

void Grid::GivePositionUpdate(Point3i gridPos) {
    Point3i diff = centerPos - gridPos;
    for (uint8_t i = 0; i < 3; i++) {
        if (abs(diff[i]) >= refreshDistance) {
            centerPos = gridPos;
            INFO("New center chunk: ", centerPos, ", active chunks: ", loadedChunks.size());
            UnloadOldChunks();
            LoadNewChunks();
        }
    }
}

void Grid::LoadNewChunks() {
    Point3i min = centerPos - gridSize;
    Point3i max = centerPos + gridSize;
    std::vector<Point3i> requestChunks;

    for (auto y = min[1]; y < max[1]; y++) {
        for (auto x = min[0]; x < max[0]; x++) {
            int32_t globHeight = architect.GetAvgGlobalHeight(Point2i(x, y));
            int32_t minZ = std::max(globHeight / CHUNK_SIZE - 1, min[2]);
            int32_t maxZ = std::min(globHeight / CHUNK_SIZE + 2, max[2]);
            for (auto z = minZ; z < maxZ; z++) {
                Point3i p(x, y, z);
                if (loadedChunks.find(p) == loadedChunks.end()) {
                    requestChunks.push_back(p);
                }
            }
        }
    }

    DEBUG("Requesting ", requestChunks.size(), " chunks for loading");
    chunkLoader.RequestChunks(requestChunks);
}

void Grid::UnloadOldChunks() {
    Point3i min = centerPos - gridSize;
    Point3i max = centerPos + gridSize;
    uint32_t unloadCount = 0;

    auto iter = loadedChunks.begin();
    while (iter != loadedChunks.end()) {
        if (!iter->first.InBoundaries(min, max)) {
            loadedChunks.erase(iter++);
            unloadCount++;
        } else {
            ++iter;
        }
    }

    DEBUG("Unloading ", unloadCount, " chunks");
}

void Grid::Update() {
    if (chunkLoader.HasFinishedChunks()) {
        //sf::Clock clock;
        std::vector<Chunk> newChunks = chunkLoader.GetFinishedChunks();
        for (auto iter = std::make_move_iterator(newChunks.begin());
             iter != std::make_move_iterator(newChunks.end()); ++iter) {
            loadedChunks.emplace(iter->GetPosition(), *iter);
        }
        //DEBUG("Updating chunks took ", clock.getElapsedTime().asMilliseconds(), "ms");
    }
}

void Grid::Draw(const Camera& camera) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
        chunkIter->second.Draw(camera);
    }
}

}   // namespace mc::world::chunk

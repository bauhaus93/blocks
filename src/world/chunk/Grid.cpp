/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(int32_t chunkDrawDistance, const Architect& architect_):
    gridSize { Point3i(chunkDrawDistance) },
    architect { architect_ },
    centerPos(1337, 1337, 1337),
    chunkLoader { 10, architect } {
    chunkLoader.Start();
}

Grid::~Grid() {
    chunkLoader.Stop();
}

void Grid::SetCenter(Point3f worldPos) {
    Point3i gridPos(worldPos / CHUNK_SIZE / BLOCK_SIZE);
    SetCenter(gridPos);
}

void Grid::SetCenter(Point3i gridPos) {
    if (centerPos != gridPos) {
        DEBUG("Changing center chunk, ", centerPos, "-> ", gridPos);
        centerPos = gridPos;
        UnloadOldChunks();
        LoadNewChunks();
    }
}

void Grid::LoadNewChunks() {
    Point3i min = centerPos - gridSize;
    Point3i max = centerPos + gridSize;
    std::vector<Point3i> requestChunks;

    for (auto y = min[1]; y < max[1]; y++) {
        for (auto x = min[0]; x < max[0]; x++) {
            int32_t globHeight = architect.GetGlobalHeight(Point2i(x, y), Point2i(CHUNK_SIZE / 2));
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

void Grid::UpdateChunks() {
    if (chunkLoader.HasFinishedChunks()) {
        //sf::Clock clock;
        std::vector<Chunk> newChunks = chunkLoader.GetFinishedChunks();
        for (auto iter = std::make_move_iterator(newChunks.begin());
             iter != std::make_move_iterator(newChunks.end()); ++iter) {
            loadedChunks.emplace(iter->GetPosition(), *iter);
        }
        //DEBUG("Updating chunks took ", clock.getElapsedTime().asMilliseconds(), "ms");
        INFO("Currently loaded chunks: ", loadedChunks.size());
    }
}

void Grid::Draw(const Camera& camera) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
        chunkIter->second.Draw(camera);
    }
}

}   // namespace mc::world::chunk

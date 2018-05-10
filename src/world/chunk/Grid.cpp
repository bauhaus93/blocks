/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(const architect::Architect& architect_):
    drawDistance { 10 },
    refreshDistance { 1 },
    architect { architect_ },
    centerPos(1337, 1337, 1337),
    chunkLoader { 20, architect } {
    chunkLoader.Start();
}

void Grid::GivePositionUpdate(Point3f worldPos) {
    Point3i gridPos(worldPos / CHUNK_SIZE / BLOCK_SIZE);
    GivePositionUpdate(gridPos);
}

void Grid::SetDrawDistance(int32_t drawDistance_) {
    drawDistance = drawDistance_;
    refreshDistance = std::max(drawDistance / 10, 1);
    UnloadOldChunks();
    LoadNewChunks();
    TRACE("Set draw distance to ", drawDistance);
}

void Grid::GivePositionUpdate(Point3i gridPos) {
    Point3i diff = centerPos - gridPos;
    for (uint8_t i = 0; i < 3; i++) {
        if (abs(diff[i]) >= refreshDistance) {
            centerPos = gridPos;
            INFO("New center chunk: ", centerPos, ", active chunks: ", loadedChunks.size());
            UnloadOldChunks();
            LoadNewChunks();
            break;
        }
    }
}

void Grid::LoadNewChunks() {
    Point3i min = centerPos - drawDistance;
    Point3i max = centerPos + drawDistance;
    std::vector<Point3i> requestChunks;

    /*min[0] = -1;
    min[1] = -1;
    max[0] = 0;
    max[1] = 0;*/
    for (auto y = min[1]; y < max[1]; y++) {
        for (auto x = min[0]; x < max[0]; x++) {
            auto pair = architect.GetMinMaxGlobalHeight(Point2i(x, y));
            int32_t minZ = std::max(pair.first / CHUNK_SIZE - 1, min[2]);
            int32_t maxZ = std::min(pair.second / CHUNK_SIZE + 2, max[2]);
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
    Point3i min = centerPos - drawDistance;
    Point3i max = centerPos + drawDistance;
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

    INFO("Unloading ", unloadCount, " chunks");
}

void Grid::Update() {
    if (chunkLoader.HasFinishedChunks()) {
        //sf::Clock clock;
        std::vector<Chunk> newChunks = chunkLoader.GetFinishedChunks();
        for (auto iter = std::make_move_iterator(newChunks.begin());
             iter != std::make_move_iterator(newChunks.end()); ++iter) {
            auto result = loadedChunks.emplace(iter->GetPosition(), *iter);
            UpdateChunkBorderFaces(result.first->second);
        }
        //DEBUG("Updating chunks took ", clock.getElapsedTime().asMilliseconds(), "ms");
    }
}

void Grid::UpdateChunkBorderFaces(Chunk& newChunk) {
    for (uint8_t i = 0; i < 6; i++) {
        auto result = loadedChunks.find(newChunk.GetPosition() + GetOffset(GetDirection(i)));
        if (result != loadedChunks.end()) {
            Chunk::UpdateBorderFaces(newChunk, result->second, GetDirection(i));
        }
    }
}

void Grid::Draw(const Camera& camera) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
        chunkIter->second.Draw(camera);
    }
}

}   // namespace mc::world::chunk

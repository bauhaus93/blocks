/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(int32_t chunkDrawDistance,
           Point3i chunkSize_,
           Point3f blockSize_):
    gridSize { Point3i(chunkDrawDistance) },
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    heightNoise { },
    centerPos(1337, 1337, 1337),
    grid { },
    texture { "grass.bmp" } {
}

void Grid::SetCenter(Point3f worldPos) {
    Point3i gridPos(worldPos / chunkSize / blockSize);
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
    constexpr uint8_t THREAD_COUNT = 10;
    std::array<std::unique_ptr<ChunkLoader>, THREAD_COUNT> loaders;
    uint8_t index = 0;

    for (auto z = -gridSize[2]; z <= gridSize[2]; z++) {
        for (auto y = -gridSize[1]; y <= gridSize[1]; y++) {
            for (auto x = -gridSize[0]; x <= gridSize[0]; x++) {
                //Point3i offset(x, y, z);
                Point3i pos = centerPos + Point3i(x, y, z);
                if (grid.find(pos) == grid.end()) {
                    if (loaders[index] == nullptr) {
                        std::unique_ptr<ChunkLoader> loader = std::make_unique<ChunkLoader>(
                            pos, chunkSize, blockSize, heightNoise, texture
                        );
                        loaders[index] = std::move(loader);
                    } else {
                        loaders[index]->Wait();
                        Chunk chunk = loaders[index]->GetChunk();
                        grid.emplace(chunk.GetPosition(), std::move(chunk));

                        std::unique_ptr<ChunkLoader> loader = std::make_unique<ChunkLoader>(
                            pos, chunkSize, blockSize, heightNoise, texture
                        );
                        loaders[index] = std::move(loader);
                        index = (index + 1) % THREAD_COUNT;
                    }
                }
            }
        }
    }
    for (uint8_t i = 0; i < THREAD_COUNT; i++) {
        loaders[i]->Wait();
        if (!loaders[i]->AlreadyRetrieved()) {
            Chunk chunk = loaders[i]->GetChunk();
            grid.emplace(chunk.GetPosition(), std::move(chunk));
        }
    }
    /*
    for (auto z = -gridSize[2]; z <= gridSize[2]; z++) {
        for (auto y = -gridSize[1]; y <= gridSize[1]; y++) {
            for (auto x = -gridSize[0]; x <= gridSize[0]; x++) {
                Point3i pos = centerPos + Point3i(x, y, z);
                grid.at(pos).CreateBorderRenderCandidates(grid);
            }
        }
    }*/
}

void Grid::UnloadOldChunks() {
    Point3i min(centerPos - gridSize);
    Point3i max(centerPos + gridSize);
    std::vector<Point3i> removePos;

    for (auto chunkIter = grid.cbegin(); chunkIter != grid.end(); ++chunkIter) {
        if (!chunkIter->first.InBoundaries(min, max)) {
            removePos.emplace_back(chunkIter->first);
        }
    }
    DEBUG("Unloading ", removePos.size(), " chunks");
    while (!removePos.empty()) {
        grid.erase(removePos.back());
        removePos.pop_back();
    }
}

void Grid::DrawBlocks(const Camera& camera, const Mesh& mesh) const {

    for (auto chunkIter = grid.cbegin(); chunkIter != grid.end(); ++chunkIter) {
        chunkIter->second.DrawBlocks(camera, mesh);
    }
}


}   // namespace mc::world::chunk

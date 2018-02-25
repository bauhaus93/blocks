/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(int32_t chunkDrawDistance,
           Point2i chunkSize_,
           Point3f blockSize_):
    size (chunkDrawDistance, chunkDrawDistance),
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    heightNoise { },
    centerPos(1337, 1337),
    grid { },
    texture { "grass.bmp" } {
}

void Grid::SetCenter(Point3f worldPos) {
    Point2i gridPos(static_cast<int32_t>(worldPos[0] / chunkSize[0] / blockSize[0]),
                    static_cast<int32_t>(worldPos[1] / chunkSize[1] / blockSize[1]));
    SetCenter(gridPos);
}

void Grid::SetCenter(Point2i gridPos) {
    if (centerPos != gridPos) {
        DEBUG("Changing center chunk, ", centerPos, "-> ", gridPos);
        centerPos = gridPos;
        UnloadOldChunks();
        LoadNewChunks();
    }
}

void Grid::LoadNewChunks() {
    for (auto y = -size[1]; y <= size[1]; y++) {
        for (auto x = -size[0]; x <= size[0]; x++) {
            Point2i pos(centerPos[0] + x,
                        centerPos[1] + y);
            if (grid.find(pos) == grid.end()) {
                Chunk chunk { pos, chunkSize, blockSize };
                chunk.Generate(heightNoise, texture);
                grid.emplace(pos, std::move(chunk));
            } 
        }
    }
}

void Grid::UnloadOldChunks() {
    Point2i min(centerPos - size);
    Point2i max(centerPos + size);
    std::vector<Point2i> removePos;

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

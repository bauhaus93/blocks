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
    for (auto z = -gridSize[2]; z <= gridSize[2]; z++) {
        for (auto y = -gridSize[1]; y <= gridSize[1]; y++) {
            for (auto x = -gridSize[0]; x <= gridSize[0]; x++) {
                //Point3i offset(x, y, z);
                Point3i pos = centerPos + Point3i(x, y, z);
                if (grid.find(pos) == grid.end()) {
                    Chunk chunk { pos, chunkSize, blockSize };
                    chunk.Generate(heightNoise, texture);
                    grid.emplace(pos, std::move(chunk));
                }
            }
        }
    }

    for (auto z = -gridSize[2]; z <= gridSize[2]; z++) {
        for (auto y = -gridSize[1]; y <= gridSize[1]; y++) {
            for (auto x = -gridSize[0]; x <= gridSize[0]; x++) {
                Point3i pos = centerPos + Point3i(x, y, z);
                grid.at(pos).CreateBorderRenderCandidates(grid);
            }
        }
    }

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

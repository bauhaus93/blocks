/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks);

Grid::Grid(int32_t chunkDrawDistance):
    gridSize { Point3i(chunkDrawDistance) },
    centerPos(1337, 1337, 1337),
    grid { },
    chunkLoader { 10 } {
    chunkLoader.Start();
}

Grid::~Grid() {
    chunkLoader.Stop();
}

void Grid::SetCenter(Point3f worldPos) {
    Point3i gridPos(worldPos / Chunk::SIZE / Block::SIZE);
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
                    chunkLoader.RequestChunk(pos);
                }
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

void Grid::UpdateChunks() {
    if (chunkLoader.HasLoadedChunks()) {
        std::vector<Chunk> newChunks = chunkLoader.GetLoadedChunks();
        while (!newChunks.empty()) {
            auto pair = grid.emplace(newChunks.back().GetPosition(),
                            std::move(newChunks.back()));
            newChunks.pop_back();
            if (!pair.first->second.IsEmpty()) {
                uncheckedBorders.emplace_back(pair.first->second);
            }
        }
    }
    CheckBorders();
}

void Grid::CheckBorders() {
    /*for(auto chunk: uncheckedBorders) {

    }*/
}



void Grid::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto chunkIter = grid.cbegin(); chunkIter != grid.end(); ++chunkIter) {
        chunkIter->second.DrawBlocks(camera, mesh);
    }
}

MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks) {
    const static std::array<Point3i, 6> offset { {
      Point3i(1, 0, 0),  Point3i(0, 1, 0),  Point3i(0, 0, 1),
      Point3i(-1, 0, 0), Point3i(0, -1, 0), Point3i(0, 0, -1),
    } };
    MapRef3D<const Chunk> neighbours;

    for (uint8_t i = 0; i < 6; i++) {
        Point3i neighbourPos = chunkPos + offset[i];
        auto find = chunks.find(neighbourPos);
        if (find != chunks.end()) {
            neighbours.emplace(offset[i], find->second);
        }
    }
    return neighbours;
}


}   // namespace mc::world::chunk

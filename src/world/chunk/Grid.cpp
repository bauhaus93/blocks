/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(int32_t chunkDrawDistance):
    gridSize { Point3i(chunkDrawDistance) },
    centerPos(1337, 1337, 1337),
    loadedChunks { },
    //chunkPosTree { nullptr },
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
        std::vector<Point3i> visibleChunks = CreateVisibleChunkPosVec();
        //RebuildChunkPosTree(visibleChunks);
        UnloadOldChunks(visibleChunks);
        LoadNewChunks(visibleChunks);
    }
}

/*void Grid::RebuildChunkPosTree(const std::list<Point3i>& visibleChunks) {
    Point3i min(centerPos - gridSize);
    Point3i max(centerPos + gridSize);
    chunkPosTree = std::make_unique<Octree<int32_t>>(min, max);
    chunkPosTree->QueueElements(visibleChunks);
    chunkPosTree->InsertQueuedElements();
}*/

bool PointIsChunkChunkPos(const Point3i& p, const Chunk& c) {
    return p == c.GetPosition();
}

void Grid::LoadNewChunks(const std::vector<Point3i>& visibleChunks) {
    std::vector<Point3i> loadChunks;

    for (auto iter = visibleChunks.begin() ;
        iter != visibleChunks.end(); ++iter) {
        auto result = std::find_if(loadedChunks.begin(),
                                   loadedChunks.end(),
                                   [iter](const Chunk& c) {
                                        return c.GetPosition() == *iter;
                                   });
        if (result == loadedChunks.end()) {
            loadChunks.push_back(*iter);
        }
    }
    DEBUG("Requesting ", loadChunks.size(), " chunks for loading");
    chunkLoader.RequestChunks(loadChunks);
}

std::vector<Point3i> Grid::CreateVisibleChunkPosVec() const {
    std::vector<Point3i> visiblePos;
    for (auto z = -gridSize[2]; z <= gridSize[2]; z++) {
        for (auto y = -gridSize[1]; y <= gridSize[1]; y++) {
            for (auto x = -gridSize[0]; x <= gridSize[0]; x++) {
                Point3i pos = centerPos + Point3i(x, y, z);
                if (pos[2] >= 0 && pos[2] < 5) {
                    visiblePos.push_back(pos);
                }
            }
        }
    }
    return visiblePos;
}

void Grid::UnloadOldChunks(const std::vector<Point3i>& visibleChunks) {
    std::vector<Chunk> keepChunks;
    
    for (auto iter = loadedChunks.begin() ;
        iter != loadedChunks.end(); ++iter) {
        auto result = std::find_if(visibleChunks.begin(),
                                   visibleChunks.end(),
                                   [iter](const Point3i& p) {
                                        return p == iter->GetPosition();
                                   });
        if (result != visibleChunks.end()) {
            keepChunks.push_back(std::move(*iter));
        }
    }   

    DEBUG("Unloading ", loadedChunks.size() - keepChunks.size(), " chunks");
    loadedChunks.swap(keepChunks);
}

void Grid::UpdateChunks() {
    if (chunkLoader.HasFinishedChunks()) {
        std::vector<Chunk> newChunks = chunkLoader.GetFinishedChunks();
        loadedChunks.insert(loadedChunks.end(),
                            std::make_move_iterator(newChunks.begin()),
                            std::make_move_iterator(newChunks.end()));
        std::sort(loadedChunks.begin(), loadedChunks.end());
        TRACE("Loaded ", newChunks.size(), " new chunks");
        
        UpdateChunkBorders();
        TRACE("Currently loaded chunks: ", loadedChunks.size());
    }
}

void Grid::UpdateChunkBorders() { 
    for(auto& chunk : loadedChunks) {
        if (!chunk.IsEmpty() && !chunk.AllNeighboursChecked()) {
            for (uint8_t i = 0; i < 6; i++) {
                Direction dir = GetDirection(i);
                if (!chunk.CheckedNeighbour(dir)) {
                    Point3i offset = GetOffset(dir);
                    Point3i neighbourPos = chunk.GetPosition() + offset;
                    auto neighbourFind = std::find_if(loadedChunks.begin(),
                                                      loadedChunks.end(),
                                                      [&neighbourPos](const Chunk& chunk) {
                                                          return chunk.GetPosition() == neighbourPos;
                                                      });
                    if (neighbourFind != loadedChunks.end()) {
                        chunk.UpdateBlockVisibility(dir, *neighbourFind);
                    }
                }
            }
        }
    }
}

void Grid::Draw(const Camera& camera) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
        chunkIter->Draw(camera);
    }
}

}   // namespace mc::world::chunk

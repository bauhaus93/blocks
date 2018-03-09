/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

Grid::Grid(int32_t chunkDrawDistance):
    gridSize { Point3i(chunkDrawDistance) },
    centerPos(1337, 1337, 1337),
    loadedChunks { },
    chunkPosTree { nullptr },
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
        std::set<Point3i> visibleChunks = CreateVisibleChunkPosSet();
        RebuildChunkPosTree(visibleChunks);
        UnloadOldChunks(visibleChunks);
        LoadNewChunks(visibleChunks);
    }
}

void Grid::RebuildChunkPosTree(const std::set<Point3i>& visibleChunks) {
    const Point3i min(centerPos - gridSize);
    const Point3i max(centerPos + gridSize);
    chunkPosTree = std::make_unique<Octree<int32_t>>(min, max);
    chunkPosTree->QueueElements(visibleChunks);
    chunkPosTree->InsertQueuedElements();
}

std::size_t Grid::GetVisibleBlocksCount() const {
	std::size_t c = 0;
    for (const auto& chunk: loadedChunks) {
        c += chunk.GetVisibleBlocksCount();
    }
    return c;
}

void Grid::LoadNewChunks(const std::set<Point3i>& visibleChunks) {
    std::set<Point3i> diff;

    std::set_difference(visibleChunks.begin(),
                        visibleChunks.end(),
                        loadedChunks.begin(),
                        loadedChunks.end(),
                        std::inserter(diff, diff.end()),
                        PointChunkCmp());
    chunkLoader.RequestChunks(diff);
    DEBUG("Requested ", diff.size(), " chunks for loading");
}

std::set<Point3i> Grid::CreateVisibleChunkPosSet() const {
    std::set<Point3i> neededPos;
    for (auto z = -gridSize[2]; z <= gridSize[2]; z++) {
        for (auto y = -gridSize[1]; y <= gridSize[1]; y++) {
            for (auto x = -gridSize[0]; x <= gridSize[0]; x++) {
                Point3i pos = centerPos + Point3i(x, y, z);
                if (pos[2] >= 0 && pos[2] < 5) {
                    neededPos.insert(pos);
                }

            }
        }
    }
    return neededPos;
}

void Grid::UnloadOldChunks(const std::set<Point3i>& visibleChunks) {
    std::vector<Chunk> keepChunks;

    std::set_intersection(std::make_move_iterator(loadedChunks.begin()),
                        std::make_move_iterator(loadedChunks.end()),
                        visibleChunks.begin(),
                        visibleChunks.end(),
                        std::inserter(keepChunks, keepChunks.end()),
                        PointChunkCmp());
    DEBUG("Unloading ", loadedChunks.size() - keepChunks.size(), " chunks");
    loadedChunks.swap(keepChunks);
}

void Grid::UpdateChunks() {
    if (chunkLoader.HasLoadedChunks()) {
        std::vector<Chunk> newChunks = chunkLoader.GetLoadedChunks();
        loadedChunks.insert(loadedChunks.end(),
                            std::make_move_iterator(newChunks.begin()),
                            std::make_move_iterator(newChunks.end()));
        std::sort(loadedChunks.begin(), loadedChunks.end());
        TRACE("Loaded ", newChunks.size(), " new chunks");
        
        CheckBorders();
        TRACE("Currently loaded chunks: ", loadedChunks.size(), ", visible blocks: ", GetVisibleBlocksCount());
    }
}

void Grid::CheckBorders() {
    std::vector<std::vector<mc::Point3i>::iterator> checkedBorders;
    
    for(auto& chunk : loadedChunks) {
        if (!chunk.IsEmpty()) {
            uint8_t checkedNeighbours = chunk.GetCheckedNeighbours();
            assert(checkedNeighbours <= 0x3F);
            if (checkedNeighbours != 0x3F) {
                for (uint8_t i = 0; i < 6; i++) {
                    if (((1 << i) & checkedNeighbours) == 0) {
                        Point3i offset(0);
                        offset[i % 3] = 1;
                        if (i < 3) {
                            offset *= -1;
                        }
                        Point3i neighbourPos = chunk.GetPosition() + offset;
                        auto neighbourFind = std::find_if(loadedChunks.begin(),
                                                        loadedChunks.end(),
                                                        [&neighbourPos](const Chunk& chunk) {
                                                            return chunk.GetPosition() == neighbourPos;
                                                        });
                        if (neighbourFind != loadedChunks.end()) {
                            //must pass the opposite mask of the neighbour for checking
                            auto mask = neighbourFind->GetSingleBorderMask((i + 3) % 6);
                            chunk.CheckNeighbour(i, mask);
                        }
                    }
                }
            }
        }
    }
}

void Grid::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
        chunkIter->DrawBlocks(camera, mesh);
    }
}

}   // namespace mc::world::chunk

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {

MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks);

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
        c += chunk.second.GetVisibleBlocksCount();
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
                neededPos.insert(pos);
            }
        }
    }
    return neededPos;
}

void Grid::UnloadOldChunks(const std::set<Point3i>& visibleChunks) {
    std::map<Point3i, Chunk> keepChunks;

    std::set_intersection(loadedChunks.begin(),
                          loadedChunks.end(),
                          visibleChunks.begin(),
                          visibleChunks.end(),
                          std::inserter(keepChunks, keepChunks.end()),
                          PointChunkCmp());
    loadedChunks.clear();
    for (auto iter = keepChunks.begin(); iter != keepChunks.end();) {
        loadedChunks.emplace(iter->first, std::move(iter->second));
    } 
    loadedChunks.swap(keepChunks);
    DEBUG("Unloaded ", loadedChunks.size() - keepChunks.size(), " chunks");

    std::set<Point3i> keepBorders;

    std::set_intersection(uncheckedBorders.begin(),
                          uncheckedBorders.end(),
                          loadedChunks.begin(),
                          loadedChunks.end(),
                          std::inserter(keepBorders, keepBorders.end()),
                          PointChunkCmp());
    uncheckedBorders.clear();
    for (auto iter = keepBorders.begin(); iter != keepBorders.end();) {
        uncheckedBorders.push_back(std::move(keepBorders.extract(iter++).value()));
    }
    DEBUG("Removed ", uncheckedBorders.size() - keepBorders.size(), " chunks from unchecked borders list");
}

void Grid::UpdateChunks() {
    if (chunkLoader.HasLoadedChunks()) {
        std::vector<Chunk> newChunks = chunkLoader.GetLoadedChunks();
        for (auto iter = newChunks.begin(); iter != newChunks.end(); iter++) {
            loadedChunks.emplace(iter->GetPosition(), std::move(*iter));
        }
        TRACE("Loaded ", newChunks.size(), " new chunks");
        
        CheckBorders();
        TRACE("Visible blocks count: ", GetVisibleBlocksCount());
    }
}

void Grid::CheckBorders() {
    std::vector<std::vector<mc::Point3i>::iterator> checkedBorders;
    
    for(auto borderIter = uncheckedBorders.begin(); borderIter != uncheckedBorders.end(); ++borderIter) {
        auto chunkFind = loadedChunks.find(*borderIter);
        assert(chunkFind != loadedChunks.end());
        Chunk& chunk = chunkFind->second;
        MapRef3D<const Chunk> neighbours = CreateImmediateNeighbourMap(chunk.GetPosition(), loadedChunks);
        uint8_t checkedNeighbours = chunk.GetCheckedNeighbours();
        for (uint8_t i = 0; i < 6; i++) {
            if (((1 << i) & checkedNeighbours) == 0) {
                Point3i offset(0);
                offset[i % 3] = 1;
                if (i < 3) {
                    offset *= -1;
                }
                auto find = neighbours.find(offset);
                if (find != neighbours.end()) {
                    //must pass the opposite mask of the neighbour for checking
                    auto mask = find->second.get().GetSingleBorderMask((i + 3) % 6);
                    chunk.CheckNeighbour(i, mask);
                }

            }
        }
        if (chunk.GetCheckedNeighbours() == 0x3F) {
            checkedBorders.push_back(borderIter);
        }
    }
    for (auto checked : checkedBorders) {
        uncheckedBorders.erase(checked);
    }
}

void Grid::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
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

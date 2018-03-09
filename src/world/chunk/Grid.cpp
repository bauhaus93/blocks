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
                neededPos.insert(pos);
            }
        }
    }
    return neededPos;
}

void Grid::UnloadOldChunks(const std::set<Point3i>& visibleChunks) {
    std::set<Point3i> loadedChunkPos;
    std::set<Point3i> unloadChunkPos;

    std::for_each(loadedChunks.begin(),
                  loadedChunks.end(),
                  [&loadedChunkPos](const Chunk& c) {
                      loadedChunkPos.insert(c.GetPosition());
                  });

    std::set_difference(loadedChunkPos.begin(),
                        loadedChunkPos.end(),
                        visibleChunks.begin(),
                        visibleChunks.end(),
                        std::inserter(unloadChunkPos, unloadChunkPos.end()));

    auto loadedBegin = std::lower_bound(loadedChunks.begin(),
                                        loadedChunks.end(),
                                        unloadChunkPos.begin());
    auto loadedEnd = std::upper_bound(loadedBegin,
                                      loadedChunks.end(),
                                      --unloadChunkPos.end());
    auto bordersBegin = std::lower_bound(uncheckedBorders.begin(),
                                         uncheckedBorders.end(),
                                         unloadChunkPos.begin());
    auto bordersEnd = std::upper_bound(bordersBegin,
                                       uncheckedBorders.end(),
                                       --unloadChunkPos.end());
    for (auto iter = unloadChunkPos.begin(); iter != unloadChunkPos.end(); ++iter) {
        std::remove_if(loadedBegin,
                       loadedEnd,
                       [&iter, &loadedBegin](const Chunk& chunk) {
                           if (chunk.GetPosition() == *iter) {
                               ++loadedBegin;
                               return true;
                           }
                       });
        std::remove_if(bordersBegin,
                       bordersEnd,
                       [&iter, &bordersBegin](const Point3i& pos) {
                           if (pos == *iter) {
                               TRACE("Removed unchecked borders entry");
                               ++bordersBegin;
                               return true;
                           }
                       });
     }

    DEBUG("Unloaded ", unloadChunkPos.size(), " chunks");
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
        TRACE("Visible blocks count: ", GetVisibleBlocksCount());
    }
}

void Grid::CheckBorders() {
    std::vector<std::vector<mc::Point3i>::iterator> checkedBorders;
    
    for(auto borderIter = uncheckedBorders.begin(); borderIter != uncheckedBorders.end(); ++borderIter) {
        auto chunkFind = std::find_if(loadedChunks.begin(),
                                      loadedChunks.end(),
                                      [&borderIter](const Chunk& c) {
                                          return c.GetPosition() == *borderIter;
                                      });
        assert(chunkFind != loadedChunks.end());
        Chunk& chunk = *chunkFind;
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

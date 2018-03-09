/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Defs.hpp"
#include "world/octree/Octree.hpp"

#include "Chunk.hpp"
#include "ChunkLoader.hpp"
#include "SimplexNoise.hpp"
#include "PointChunkCmp.hpp"

namespace mc::world::chunk {

class Grid {

 public:
    explicit        Grid(int32_t chunkDrawDistance);
                    ~Grid();

    void            SetCenter(Point3f worldPos);
	std::size_t		GetVisibleBlocksCount() const;
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;
    void            UpdateChunks();


 private:
    void            SetCenter(Point3i centerPos);
    void            LoadNewChunks(const std::set<Point3i>& visibleChunks);
    void            UnloadOldChunks(const std::set<Point3i>& visibleChunks);
    void            CheckBorders();
    void            RebuildChunkPosTree(const std::set<Point3i>& visibleChunks);
    std::set<Point3i> CreateVisibleChunkPosSet() const;

    Point3i                     gridSize;
    Point3i                     centerPos;
    std::vector<Chunk>          loadedChunks;
    OctreePtrI                  chunkPosTree;
    ChunkLoader                 chunkLoader;

};

}   // namespace mc::world::chunk

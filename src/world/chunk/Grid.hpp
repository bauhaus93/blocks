/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Defs.hpp"
#include "world/Octree.hpp"

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
    void            Draw(const Camera& camera) const;
    void            UpdateChunks();

 private:
    void            SetCenter(Point3i centerPos);
    void            LoadNewChunks(const std::vector<Point3i>& visibleChunks);
    void            UnloadOldChunks(const std::vector<Point3i>& visibleChunks);
    void            UpdateChunkBorders();
    //void            RebuildChunkPosTree(const std::vector<Point3i>& visibleChunks);
    std::vector<Point3i> CreateVisibleChunkPosVec() const;

    Point3i                     gridSize;
    Point3i                     centerPos;
    std::vector<Chunk>          loadedChunks;
    //OctreePtrI                  chunkPosTree;
    ChunkLoader                 chunkLoader;

};

}   // namespace mc::world::chunk

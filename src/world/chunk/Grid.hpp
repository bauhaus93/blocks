/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>
#include <set>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Defs.hpp"
#include "world/octree/Octree.hpp"

#include "Chunk.hpp"
#include "ChunkLoader.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::chunk {

class Grid {

 public:
    explicit        Grid(int32_t chunkDrawDistance);
                    ~Grid();

    void            SetCenter(Point3f worldPos);
	size_t			GetVisibleBlocksCount() const;
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;
    void            UpdateChunks();


 private:
    void            SetCenter(Point3i centerPos);
    void            LoadNewChunks();
    void            UnloadOldChunks();
    void            CheckBorders();
    std::set<Point3i> CreateVisibleChunkPosSet() const;

    Point3i             gridSize;
    Point3i             centerPos;
    VecRef<Chunk>       uncheckedBorders;
    std::set<Chunk>     loadedChunks;
    OctreePtrI          chunkPosTree;
    ChunkLoader         chunkLoader;

};

}   // namespace mc::world::chunk

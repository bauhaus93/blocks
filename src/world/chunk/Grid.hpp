/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>
#include <array>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Defs.hpp"
#include "world/Octree.hpp"
#include "world/SimplexNoise.hpp"
#include "world/HeightCalculation.hpp"

#include "Chunk.hpp"
#include "ChunkLoader.hpp"
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
    void            LoadNewChunks();
    void            UnloadOldChunks();
    void            UpdateChunkBorders();
    std::vector<Point3i> CreateVisibleChunkPosVec() const;

    Point3i                     gridSize;
    Point3i                     centerPos;
    SimplexNoise                heightNoise;
    std::map<Point3i, Chunk>    loadedChunks;
    std::map<Point2i, int32_t>  heightEstaminations;
    ChunkLoader                 chunkLoader;

};

}   // namespace mc::world::chunk

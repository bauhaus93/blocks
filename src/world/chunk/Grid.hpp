/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>
#include <array>


#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Defs.hpp"
#include "graphics/TextureAtlas.hpp"
#include "world/Octree.hpp"
#include "world/Architect.hpp"
#include "world/Size.hpp"
#include "ApplicationError.hpp"
#include "Chunk.hpp"
#include "ChunkLoader.hpp"

namespace mc::world::chunk {

class Grid {

 public:
    explicit        Grid(int32_t chunkDrawDistance, const Architect& architect_, const graphics::TextureAtlas& atlas);
                    ~Grid();

    void            SetCenter(Point3f worldPos);
    void            Draw(const Camera& camera) const;
    void            UpdateChunks();

 private:
    void            SetCenter(Point3i centerPos);
    void            LoadNewChunks();
    void            UnloadOldChunks();
    std::vector<Point3i> CreateVisibleChunkPosVec() const;

    Point3i                     gridSize;
    const Architect&            architect;
    Point3i                     centerPos;
    std::map<Point3i, Chunk>    loadedChunks;
    ChunkLoader                 chunkLoader;

};

}   // namespace mc::world::chunk

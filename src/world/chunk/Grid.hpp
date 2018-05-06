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
#include "world/architect/Architect.hpp"
#include "world/Size.hpp"
#include "Chunk.hpp"
#include "ChunkLoader.hpp"

namespace mc::world::chunk {

class Grid {

 public:
    explicit        Grid(const architect::Architect& architect_);

    void            GivePositionUpdate(Point3f worldPos);
    void            SetDrawDistance(int32_t drawDistance_);
    void            Draw(const Camera& camera) const;
    void            Update();

    int32_t         GetDrawDistance() const { return drawDistance; }

 private:
    void            GivePositionUpdate(Point3i centerPos);
    void            LoadNewChunks();
    void            UnloadOldChunks();
    std::vector<Point3i> CreateVisibleChunkPosVec() const;

    int32_t                                 drawDistance;
    int32_t                                 refreshDistance;
    const architect::Architect&             architect;
    Point3i                                 centerPos;
    std::map<Point3i, Chunk>                loadedChunks;
    ChunkLoader                             chunkLoader;

};

}   // namespace mc::world::chunk

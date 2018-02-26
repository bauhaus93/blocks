/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"

#include "Chunk.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::chunk {

typedef std::map<Point3i, Chunk> ChunkMap;

class Grid {

 public:
                    Grid(int32_t chunkDrawDistance,
                         Point3i chunkSize_,
                         Point3f blockSize_);

    void            SetCenter(Point3f worldPos);
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;

 private:
    void                SetCenter(Point3i centerPos);
    void                LoadNewChunks();
    void                UnloadOldChunks();

    Point3i             gridSize;
    Point3i             chunkSize;
    Point3f             blockSize;
    SimplexNoise        heightNoise;
    Point3i             centerPos;
    ChunkMap            grid;

    Texture     	texture;
};

}   // namespace mc::world::chunk

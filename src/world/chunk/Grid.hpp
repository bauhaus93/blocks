/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"

#include "Chunk.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::chunk {

typedef std::map<Point2i, Chunk> ChunkMap;

class Grid {

 public:
                    Grid(int32_t chunkDrawDistance,
                         Point2i chunkSize_,
                         Point3f blockSize_);

    void            SetCenter(Point3f worldPos);
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;

 private:
    void                SetCenter(Point2i centerPos);
    void                LoadNewChunks();
    void                UnloadOldChunks();

    Point2i             size;
    Point2i             chunkSize;
    Point3f             blockSize;
    SimplexNoise        heightNoise;
    Point2i             centerPos;
    ChunkMap            grid;
    
    Texture     	texture;
};

}   // namespace mc::world::chunk

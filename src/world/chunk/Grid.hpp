/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>
#include <memory>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"

#include "Chunk.hpp"
#include "ChunkLoader.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::chunk {

class Grid {

 public:
                    Grid(int32_t chunkDrawDistance,
                         Point3i chunkSize_,
                         Point3f blockSize_);
                    ~Grid();

    void            SetCenter(Point3f worldPos);
    void            DrawBlocks(const Camera& camera, const Mesh& mesh) const;
    void            UpdateChunks();

 private:
    void                SetCenter(Point3i centerPos);
    void                LoadNewChunks();
    void                UnloadOldChunks();


    Point3i             chunkSize;
    Point3f             blockSize;
    Point3i             gridSize;
    Point3i             centerPos;
    Map3D<Chunk>        grid;
    ChunkLoader         chunkLoader;

};

}   // namespace mc::world::chunk

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"


namespace mc {


Chunk::Chunk(const Position& origin_,
             const Point3<float>& blockSize_,
             const Point3<uint32_t>& gridSize_):
    origin { origin_ },
    blockSize { blockSize_ },
    gridSize { gridSize_ },
    grid { } {
}


void Chunk::Generate(const Mesh& mesh, const Texture& texture) {

    for (int z = 0; z < gridSize[2]; z++) {
        for (int y = 0; y < gridSize[1]; y++) {
            for (int x = 0; x < gridSize[0]; x++) {
                Point3<uint32_t> gridPos(x, y, z);
                Position worldPos(origin[0] + x * blockSize[0],
                                  origin[1] + y * blockSize[1],
                                  origin[2] + z * blockSize[2]);  
                grid.emplace(gridPos, Cube (worldPos, mesh, texture));
            }
        }
    }
}

void Chunk::Draw(const Camera& camera) const {
    for (auto iter = grid.begin(); iter != grid.end(); ++iter) {
        iter->second.Draw(camera);
    }
}


}       // namespace mc

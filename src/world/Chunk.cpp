/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"


namespace mc {


Chunk::Chunk(const Position& origin_,
             const Point3<float>& blockSize_,
             const Point3<uint32_t>& gridSize_):
    origin { origin_ },
    blockSize { blockSize_ },
    gridSize { gridSize_ },
    grid { },
    renderCandidates { } {
}


void Chunk::Generate(const Mesh& mesh, const Texture& texture) {

    for (int z = 0; z < gridSize[2]; z++) {
        for (int y = 0; y < gridSize[1]; y++) {
            for (int x = 0; x < gridSize[0]; x++) {
                Point3<int32_t> gridPos(x, y, z);
                Position worldPos(origin[0] + x * blockSize[0],
                                  origin[1] + y * blockSize[1],
                                  origin[2] + z * blockSize[2]);  
                grid.emplace(gridPos, Cube (worldPos, mesh, texture));
            }
        }
    }
    CreateRenderCandidates();
}

void Chunk::CreateRenderCandidates() {
    const static Point3<int32_t> offset[6] {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { -1, 0, 0 },
        { 0, -1, 0 },
        { 0, 0, 1 },
        { 0, 0, -1 }
    };

    renderCandidates.clear();
    for (auto iter = grid.begin(); iter != grid.end(); ++iter) {
        auto& block = iter->second;
        auto& gridPos = iter->first;
        int neighbours = 0;
        for (int i = 0; i < 6; i++) {
            if (grid.find(gridPos + offset[i]) != grid.end()) {
                neighbours++;
            } else {
                break;
            }
        }
        if (neighbours < 6) {
            renderCandidates.emplace_back(block);
        }
    }
    DEBUG("Render candidates in chunk: ", renderCandidates.size());
}

void Chunk::Draw(const Camera& camera) const {
    for (auto iter = renderCandidates.begin(); iter != renderCandidates.end(); ++iter) {
        iter->get().Draw(camera);
    }
}


}       // namespace mc

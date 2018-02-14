/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"


namespace mc {


Chunk::Chunk(const Point2<int32_t>& chunkPos_,
             const Point3<float>& blockSize_,
             const Point2<int32_t>& gridSize_):
    chunkPos { chunkPos_ },
    blockSize { blockSize_ },
    gridSize { gridSize_ },
    origin { chunkPos[0] * blockSize[0] * gridSize[0],
             chunkPos[1] * blockSize[1] * gridSize[1],
             0.0f },
    grid { },
    renderCandidates { } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    blockSize { other.blockSize },
    gridSize { other.gridSize },
    origin { other.origin },
    grid { std::move(other.grid) },
    renderCandidates { std::move(other.renderCandidates) } {
    TRACE("Chunk move constructor");
}


void Chunk::Generate(const SimplexNoise& noise, const Mesh& mesh, const Texture& texture) {
    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    sf::Clock clock;

    for (auto y = 0; y < gridSize[1]; y++) {
        for (auto x = 0; x < gridSize[0]; x++) {
            double normalizedNoise = (1.0 + noise.GetOctavedNoise(chunkPos[0] * gridSize[0] + x,
                                                                  chunkPos[1] * gridSize[1] + y,
                                                                  6, 0.1, 0.025)) / 2.0;
            int32_t height = static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);
            GenerateColumn(Point3<int32_t>(x, y, height), mesh, texture);
        }
    }
    CreateRenderCandidates();
    DEBUG("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", grid.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3<int32_t> top, const Mesh& mesh, const Texture& texture) {
    for (int32_t z = top[2]; z  >= 0; z--) {
        Point3<int32_t> gridPos(top[0], top[1], z);
        Position worldPos(origin[0] + top[0] * blockSize[0],
                          origin[1] + top[1] * blockSize[1],
                          origin[2] + static_cast<int32_t>(z) * blockSize[2]);
        grid.emplace(gridPos, Cube (worldPos, mesh, texture));
    }
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
}

void Chunk::Draw(const Camera& camera) const {
    for (auto iter = renderCandidates.begin(); iter != renderCandidates.end(); ++iter) {
        iter->get().Draw(camera);
    }
}


}       // namespace mc

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

Chunk::Chunk(const Point2i& chunkPos_,
             const Point2i& chunkSize_,
             const Point3f& blockSize_):
    chunkPos { chunkPos_ },
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    origin { chunkPos[0] * blockSize[0] * chunkSize[0],
             chunkPos[1] * blockSize[1] * chunkSize[1],
             0.0f },
    blocks { },
    renderCandidates { } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    chunkSize { other.chunkSize },
    blockSize { other.blockSize },
    origin { other.origin },
    blocks { std::move(other.blocks) },
    renderCandidates { std::move(other.renderCandidates) } {
}

void Chunk::Generate(const SimplexNoise& noise, const Texture& texture) {
    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    sf::Clock clock;

    for (auto y = 0; y < chunkSize[1]; y++) {
        for (auto x = 0; x < chunkSize[0]; x++) {
            double normalizedNoise = (1.0 + noise.GetOctavedNoise(chunkPos[0] * chunkSize[0] + x,
                                                                  chunkPos[1] * chunkSize[1] + y,
                                                                  6, 0.1, 0.025)) / 2.0;
            int32_t height = static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);
            GenerateColumn(Point3i(x, y, height), texture);
        }
    }
    CreateRenderCandidates();
    DEBUG("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture) {
    for (int32_t z = top[2]; z  >= 0; z--) {
        Point3i gridPos(top[0], top[1], z);
        Position worldPos(origin[0] + top[0] * blockSize[0],
                          origin[1] + top[1] * blockSize[1],
                          origin[2] + static_cast<int32_t>(z) * blockSize[2]);
        blocks.emplace(gridPos, Cube (worldPos, texture));
    }
}

void Chunk::CreateRenderCandidates() {
    const static std::array<Point3i, 6> offset { {
        Point3i(1, 0, 0),
        Point3i(0, 1, 0),
        Point3i(-1, 0, 0),
        Point3i(0, -1, 0),
        Point3i(0, 0, 1),
        Point3i(0, 0, -1)
    } };

    renderCandidates.clear();
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        auto& block = iter->second;
        auto& blockPos = iter->first;
        int neighbours = 0;
        for (int i = 0; i < 6; i++) {
            if (blocks.find(blockPos + offset[i]) != blocks.end()) {
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

void Chunk::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto iter = renderCandidates.begin(); iter != renderCandidates.end(); ++iter) {
        iter->get().Draw(camera, mesh);
    }
}

}       // namespace mc::world::chunk

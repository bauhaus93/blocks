/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

Chunk::Chunk(const Point3i& chunkPos_,
             const Point3i& chunkSize_,
             const Point3f& blockSize_):
    chunkPos { chunkPos_ },
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    origin { chunkPos * chunkSize * blockSize },
    blocks { },
    renderCandidates { } {
    DEBUG("New Chunk: chunk pos = ", chunkPos, ", origin = ", origin);
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
            if (origin[2] < 0.0f) {
                height = chunkSize[2];
            } else {
                height -= origin[2];
            }
            if (height > 0) {
                GenerateColumn(Point3i(x, y, height), texture);
            }
        }
    }
    CreateRenderCandidates();
    DEBUG("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture) {
    while (top[2] > 0) {
        Point3f localPos = Point3f(top) * blockSize;
        Point3f worldPos = origin + localPos;
        blocks.emplace(localPos, Cube (worldPos, texture));
        top[2] = top[2] - 1;
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
    const static Point3i boundaryMin(0, 0, 0);
    const static Point3i boundaryMax = chunkSize - 1;

    renderCandidates.clear();
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {    //TODO currently disabled
        renderCandidates.emplace_back(iter->second);
        continue;

        Cube& block = iter->second;
        const Point3i& blockPos = iter->first;
        int neighbours = 0;
        for (int i = 0; i < 6; i++) {
            Point3i neighbour = blockPos + offset[i];
            if (!neighbour.InBoundaries(boundaryMin, boundaryMax)) {      // assumes block in next chunk is existing (must not be true),
                neighbours++;                                           // TODO make better
            } else if (blocks.find(neighbour) != blocks.end()) {
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

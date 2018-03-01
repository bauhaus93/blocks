/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

bool IsBorderBlock(const Point3i& blockPos, const Point3i& chunkSize);
bool IsCompletelyHiddenBlock(const Point3i& blockPos, const Map3D<Cube>& blocks);
uint8_t IsCompletelyHiddenBorderBlock(const Point3i& blockPos,
                                      const Map3D<Cube>& blocks,
                                      MapRef3D<const Chunk>& chunkNeighbours,
                                      const Point3i& chunkSize);
uint8_t CountNeighbours(const Point3i& blockPos, const Map3D<Cube>& blocks);
MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks);

Chunk::Chunk(const Point3i& chunkPos_,
             const Point3i& chunkSize_,
             const Point3f& blockSize_):
    chunkPos { chunkPos_ },
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    origin { chunkPos * chunkSize * blockSize },
    blocks { },
    renderCandidates { },
    renderCandidatesBorder { } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    chunkSize { other.chunkSize },
    blockSize { other.blockSize },
    origin { other.origin },
    blocks { std::move(other.blocks) },
    renderCandidates { std::move(other.renderCandidates) },
    renderCandidatesBorder { std::move(other.renderCandidatesBorder) } {
}

bool Chunk::IsEmpty() const {
    return blocks.size() == 0;
}

bool Chunk::BlockExists(const Point3i& blockPos) const {
    return blocks.find(blockPos) != blocks.end();
}

void Chunk::Generate(const SimplexNoise& noise, const Texture& texture) {
    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    sf::Clock clock;

    int32_t chunkMinHeight = chunkPos[2] * chunkSize[2];    //incl
    int32_t chunkMaxHeight = chunkMinHeight + chunkSize[2]; //excl

    for (auto y = 0; y < chunkSize[1]; y++) {
        for (auto x = 0; x < chunkSize[0]; x++) {
            double normalizedNoise = (1.0 + noise.GetOctavedNoise(chunkPos[0] * chunkSize[0] + x,
                                                                  chunkPos[1] * chunkSize[1] + y,
                                                                  6, 0.1, 0.025)) / 2.0;
            //height given in global block granularity
            int32_t height = static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);

            if (height >= chunkMinHeight && height < chunkMaxHeight) {
                height -= chunkMinHeight;   // part air/underground col
            } else if (height >= chunkMaxHeight) {
                height = chunkSize[2] - 1;      // full underground col
            } else {
                height = 0;                 // full air col
            }
            if (height > 0) {
                GenerateColumn(Point3i(x, y, height), texture);
            }
        }
    }
    CreateNonBorderRenderCandidates();
    TRACE("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture) {
    while (top[2] >= 0) {
        Point3f worldPos = origin + top * blockSize;
        blocks.emplace(top, Cube (worldPos, texture));
        top[2] = top[2] - 1;
    }
}

void Chunk::CreateNonBorderRenderCandidates() {
    renderCandidates.clear();
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        const Point3i& blockPos = iter->first;
        if (!IsBorderBlock(blockPos, chunkSize)) {
            if (!IsCompletelyHiddenBlock(blockPos, blocks)) {
                renderCandidates.emplace_back(iter->second);
            }
        }
    }
}

void Chunk::CreateBorderRenderCandidates(const Map3D<Chunk>& chunks) {
    renderCandidatesBorder.clear();

    MapRef3D<const Chunk> neighbours = CreateImmediateNeighbourMap(chunkPos, chunks);

    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        const Point3i& blockPos = iter->first;
        if (IsBorderBlock(blockPos, chunkSize)) {
            if (!IsCompletelyHiddenBorderBlock(blockPos,
                                              blocks,
                                              neighbours,
                                              chunkSize)) {
                renderCandidatesBorder.emplace_back(iter->second);
            }
        }
    }
}

void Chunk::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto iter = renderCandidates.begin(); iter != renderCandidates.end(); ++iter) {
        iter->get().Draw(camera, mesh);
    }
    for (auto iter = renderCandidatesBorder.begin(); iter != renderCandidatesBorder.end(); ++iter) {
        iter->get().Draw(camera, mesh);
    }
}

bool IsBorderBlock(const Point3i& blockPos, const Point3i& chunkSize) {
    for (uint8_t i = 0; i < 3; i++) {
        if (blockPos[i] == 0) {
            return true;
        } else if (blockPos[i] == chunkSize[i] - 1) {
            return true;
        }
    }
    return false;
}

bool IsCompletelyHiddenBlock(const Point3i& blockPos, const Map3D<Cube>& blocks) {
    const static std::array<Point3i, 6> offset { {
        Point3i(1, 0, 0),  Point3i(0, 1, 0),  Point3i(0, 0, 1),
        Point3i(-1, 0, 0), Point3i(0, -1, 0), Point3i(0, 0, -1),
    } };
    uint8_t neighbours = 0;
    for (uint8_t i = 0; i < 6; i++) {
        Point3i neighbour = blockPos + offset[i];
        if (blocks.find(neighbour) == blocks.end()) {
            break;
        } else {
            neighbours++;
        }
    }
    return neighbours == 6;
}

uint8_t IsCompletelyHiddenBorderBlock(const Point3i& blockPos,
                                      const Map3D<Cube>& blocks,
                                      MapRef3D<const Chunk>& chunkNeighbours,
                                      const Point3i& chunkSize) {
    uint8_t neighbourCount = CountNeighbours(blockPos, blocks);
    if (neighbourCount < 3) {
        return false;
    }
    for (uint8_t i = 0; i < 3; i++) {
        if (blockPos[i] == 0 || blockPos[i] == chunkSize[i] - 1) {
            Point3i neighbourOffset(0, 0, 0);
            if (blockPos[i] == 0) {
                neighbourOffset[i] = -1;
            } else {
                neighbourOffset[i] = 1;
            }
            auto neighbourChunk = chunkNeighbours.find(neighbourOffset);
            if (neighbourChunk != chunkNeighbours.end()) {
                Point3i neighbourBlockPos(blockPos);
                if (blockPos[i] == 0) {
                    neighbourBlockPos[i] = chunkSize[i] - 1;
                } else {
                    neighbourBlockPos[i] = 0;
                }
                if (neighbourChunk->second.get().BlockExists(neighbourBlockPos)) {
                    neighbourCount++;
                } else {
                    break;
                }
            } else {
                neighbourCount++;   // neighbour chunk not yet loaded, assume it's a neighbour
            }
        }
    }
    return neighbourCount == 6;
}

uint8_t CountNeighbours(const Point3i& blockPos, const Map3D<Cube>& blocks) {
    const static std::array<Point3i, 6> offset { {
        Point3i(1, 0, 0),  Point3i(0, 1, 0),  Point3i(0, 0, 1),
        Point3i(-1, 0, 0), Point3i(0, -1, 0), Point3i(0, 0, -1),
    } };
    uint8_t neighbours = 0;
    for (uint8_t i = 0; i < 6; i++) {
        Point3i neighbour = blockPos + offset[i];
        if (blocks.find(neighbour) != blocks.end()) {
            neighbours++;
        }
    }
    return neighbours;
}

MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks) {
    const static std::array<Point3i, 6> offset { {
      Point3i(1, 0, 0),  Point3i(0, 1, 0),  Point3i(0, 0, 1),
      Point3i(-1, 0, 0), Point3i(0, -1, 0), Point3i(0, 0, -1),
    } };
    MapRef3D<const Chunk> neighbours;

    for (uint8_t i = 0; i < 6; i++) {
        Point3i neighbourPos = chunkPos + offset[i];
        auto find = chunks.find(neighbourPos);
        if (find != chunks.end()) {
            neighbours.emplace(offset[i], find->second);
        }
    }
    return neighbours;
}

}       // namespace mc::world::chunk

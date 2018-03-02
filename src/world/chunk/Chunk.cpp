/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

bool IsBorderBlock(const Point3i& blockPos);
bool IsCompletelyHiddenBlock(const Point3i& blockPos, const Map3D<Block>& blocks);
uint8_t IsCompletelyHiddenBorderBlock(const Point3i& blockPos,
                                      const Map3D<Block>& blocks,
                                      MapRef3D<const Chunk>& chunkNeighbours);
uint8_t CountNeighbours(const Point3i& blockPos, const Map3D<Block>& blocks);
MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks);

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * Chunk::SIZE * Block::SIZE },
    blocks { },
    renderCandidates { },
    renderCandidatesBorder { } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
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

/* measurements:
    70-100 ms for 4096 block (Block visibility checked after generation is done)
*/
void Chunk::Generate(const SimplexNoise& noise, const Texture& texture) {
    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    sf::Clock clock;

    int32_t chunkMinHeight = chunkPos[2] * Chunk::SIZE;    //incl
    int32_t chunkMaxHeight = chunkMinHeight + Chunk::SIZE; //excl

    //VecRef<Block> currLine;
    //currLine.resize(chunkSize[0]);
    for (auto y = 0; y < Chunk::SIZE; y++) {
        for (auto x = 0; x < Chunk::SIZE; x++) {
            double normalizedNoise = (1.0 + noise.GetOctavedNoise(chunkPos[0] * Chunk::SIZE + x,
                                                                  chunkPos[1] * Chunk::SIZE + y,
                                                                  6, 0.1, 0.025)) / 2.0;
            //height given in global block granularity
            int32_t height = static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);

            if (height >= chunkMinHeight && height < chunkMaxHeight) {
                height -= chunkMinHeight;   // part air/underground col
            } else if (height >= chunkMaxHeight) {
                height = Chunk::SIZE - 1;      // full underground col
            } else {
                height = 0;                 // full air col
            }
            if (height > 0) {
                GenerateColumn(Point3i(x, y, height), texture);
            }
        }
    }
    CreateNonBorderRenderCandidates();
    DEBUG("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture) {
    Point3f worldPos = origin + top * Block::SIZE;
    Block block(worldPos, texture);
    blocks.emplace(top, Block(worldPos, texture));
    top[2] = top[2] - 1;
    while (top[2] >= 0) {
        Point3f worldPos = origin + top * Block::SIZE;
        blocks.emplace(top, Block(worldPos, texture));
        top[2] = top[2] - 1;
    }
}

void Chunk::CreateNonBorderRenderCandidates() {
    renderCandidates.clear();
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        const Point3i& blockPos = iter->first;
        if (!IsBorderBlock(blockPos)) {
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
        if (IsBorderBlock(blockPos)) {
            if (!IsCompletelyHiddenBorderBlock(blockPos,
                                              blocks,
                                              neighbours)) {
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

bool IsBorderBlock(const Point3i& blockPos) {
    for (uint8_t i = 0; i < 3; i++) {
        if (blockPos[i] == 0) {
            return true;
        } else if (blockPos[i] == Chunk::SIZE - 1) {
            return true;
        }
    }
    return false;
}

bool IsCompletelyHiddenBlock(const Point3i& blockPos, const Map3D<Block>& blocks) {
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
                                      const Map3D<Block>& blocks,
                                      MapRef3D<const Chunk>& chunkNeighbours) {
    uint8_t neighbourCount = CountNeighbours(blockPos, blocks);
    if (neighbourCount < 3) {
        return false;
    }
    for (uint8_t i = 0; i < 3; i++) {
        if (blockPos[i] == 0 || blockPos[i] == Chunk::SIZE - 1) {
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
                    neighbourBlockPos[i] = Chunk::SIZE - 1;
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

uint8_t CountNeighbours(const Point3i& blockPos, const Map3D<Block>& blocks) {
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

/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

MapRef3D<const Chunk> CreateImmediateNeighbourMap(const Point3i& chunkPos,
                                                  const Map3D<Chunk>& chunks);
void CalculateHeights(HeightArray& height,
                    const Point3i chunkPos,
                    const SimplexNoise& heightNoise);

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * Chunk::SIZE * Block::SIZE },
    blocks { },
    renderCandidates { } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    origin { other.origin },
    blocks { std::move(other.blocks) },
    renderCandidates { std::move(other.renderCandidates) } {
}

bool Chunk::IsEmpty() const {
    return blocks.size() == 0;
}

bool Chunk::BlockExists(const Point3i& blockPos) const {
    return blocks.find(blockPos) != blocks.end();
}

/* measurements:
    70-100 ms (full 4096 chunk): (OLD) Block visibility checked after generation is done (through map accesses), excluding border blocks
    ~60 ms    (full 4096): (CURRENT) Visibility saved in block, calculated during building (partly using raw pointers), includes border blocks, which need additionial checking
*/
void Chunk::Generate(const SimplexNoise& noise, const Texture& texture) {
    sf::Clock clock;

    HeightArray relativeHeight;
    CalculateHeights(relativeHeight, chunkPos, noise);

    int32_t* currHeight = &relativeHeight[0];
    for (auto y = 0; y < Chunk::SIZE; y++) {
        for (auto x = 0; x < Chunk::SIZE; x++) {
            uint8_t columnNeighbours = 0;

            if (x > 0 && *(currHeight - 1) >= *currHeight) {
                columnNeighbours++;
            }
            if (y > 0 && *(currHeight - Chunk::SIZE) >= *currHeight) {
                columnNeighbours++;
            }
            if (x + 1 < Chunk::SIZE && *(currHeight + 1) >= *currHeight) {
                columnNeighbours++;
            }
            if (y + 1 < Chunk::SIZE && *(currHeight + Chunk::SIZE) >= *currHeight) {
                columnNeighbours++;
            }
            GenerateColumn(Point3i(x, y, *currHeight), texture, columnNeighbours);
            currHeight++;
        }
    }

    DEBUG("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture, uint8_t neighbours) {
    Point3i curr(top);
    while (curr[2] >= 0) {
        Point3f worldPos = origin + curr * Block::SIZE;
        if (curr[2] == top[2] || curr[2] == 1) {
            auto pair = blocks.emplace(curr, Block(worldPos, texture, neighbours + 1));
            if (neighbours + 1 < 6) {
                renderCandidates.emplace_back(pair.first->second);
            }
        } else {
            auto pair = blocks.emplace(curr, Block(worldPos, texture, neighbours + 2));
            if (neighbours + 2 < 6) {
                renderCandidates.emplace_back(pair.first->second);
            }
        }
        curr[2] = curr[2] - 1;
    }
}

void Chunk::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto iter = renderCandidates.begin(); iter != renderCandidates.end(); ++iter) {
        iter->get().Draw(camera, mesh);
    }
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

void CalculateHeights(HeightArray& height,
                      const Point3i chunkPos,
                      const SimplexNoise& heightNoise) {

    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    int32_t chunkMinHeight = chunkPos[2] * Chunk::SIZE;    //incl
    int32_t chunkMaxHeight = chunkMinHeight + Chunk::SIZE; //excl
    int32_t* currHeight = &height[0];

    for (auto y = 0; y < Chunk::SIZE; y++) {
        for (auto x = 0; x < Chunk::SIZE; x++) {
            double normalizedNoise = (1.0 + heightNoise.GetOctavedNoise(
                chunkPos[0] * Chunk::SIZE + x,
                chunkPos[1] * Chunk::SIZE + y,
                6, 0.1, 0.025)) / 2.0;
            //height given in global block granularity
            *currHeight = static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);

            if (*currHeight >= chunkMinHeight && *currHeight < chunkMaxHeight) {
                *currHeight -= chunkMinHeight;       // part air/underground col
            } else if (*currHeight >= chunkMaxHeight) {
                *currHeight = Chunk::SIZE - 1;      // full underground col
            } else {
                *currHeight = -1;                     // full air col
            }
            currHeight++;
        }
    }
}

}       // namespace mc::world::chunk

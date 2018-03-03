/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

void CalculateHeights(HeightArray& height,
                    const Point3i chunkPos,
                    const SimplexNoise& heightNoise);

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * Chunk::SIZE * Block::SIZE },
    neighbourChecked { 0 },
    blocks { },
    renderCandidates { } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    origin { other.origin },
    neighbourChecked { other.neighbourChecked },
    blocks { std::move(other.blocks) },
    renderCandidates { std::move(other.renderCandidates) } {
}

bool Chunk::IsEmpty() const {
    return blocks.size() == 0;
}

bool Chunk::IsFull() const {
    return blocks.size() == Chunk::SIZE * Chunk::SIZE;
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
            //border column neigbours in other chunks are considered having SIZE height
            //assuming they're full of blocks -> false positives occur (visible blocks classified invisible)
            std::array<int32_t, 4> neighbourHeight = { Chunk::SIZE,
                                                       Chunk::SIZE,
                                                       Chunk::SIZE,
                                                       Chunk::SIZE
                                                     };
            if (x > 0) {
                neighbourHeight[0] = *(currHeight - 1);
            }
            if (y > 0) {
                neighbourHeight[1] = *(currHeight - Chunk::SIZE);
            }
            if (x + 1 < Chunk::SIZE) {
                neighbourHeight[2] = *(currHeight + 1);
            }
            if (y + 1 < Chunk::SIZE) {
                neighbourHeight[3] = *(currHeight + Chunk::SIZE);
            }
            GenerateColumn(Point3i(x, y, *currHeight), texture, neighbourHeight);
            currHeight++;
        }
    }

    DEBUG("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size(),
          ", renderable blocks: ", renderCandidates.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture, std::array<int32_t, 4>& neighbourHeight) {
    Point3i curr(top);

    while (curr[2] >= 0) {
        Point3f worldPos = origin + curr * Block::SIZE;
        uint8_t neighbours = 0;
        if (curr[2] == top[2] && top[2] != Chunk::SIZE - 1) {
            neighbours += 1;
        } else {
            neighbours += 2;
        }
        for (auto nHeight : neighbourHeight) {
            if (curr[2] <= nHeight) {
                neighbours++;
            }
        }
        auto pair = blocks.emplace(curr, Block(worldPos, texture, neighbours));
        if (neighbours < 6) {
            renderCandidates.emplace_back(pair.first->second);
        }

        curr[2] = curr[2] - 1;
    }
}

void Chunk::DrawBlocks(const Camera& camera, const Mesh& mesh) const {
    for (auto iter = renderCandidates.begin(); iter != renderCandidates.end(); ++iter) {
        iter->get().Draw(camera, mesh);
    }
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

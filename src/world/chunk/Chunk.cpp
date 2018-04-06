/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

static void CalculateHeights(HeightArray& height,
                    const Point3i chunkPos,
                    const Architect& architect);
//bool IsBorderBlock(const Point3i& blockPos);

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE * BLOCK_SIZE) },
    model {  CreateTranslationMatrix(origin) * glm::mat4(1.0f) },
    checkedNeighbours { },
    blocks { },
    mesh { nullptr } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    origin { other.origin },
    model { std::move(other.model) },
    checkedNeighbours { other.checkedNeighbours },
    blocks { std::move(other.blocks) },
    mesh { std::move(other.mesh) } {
}

Chunk& Chunk::operator=(Chunk&& other) {
    chunkPos = other.chunkPos;
    origin = other.origin;
    model = std::move(other.model);
    checkedNeighbours = other.checkedNeighbours;
    blocks = std::move(other.blocks);
    mesh = std::move(other.mesh);
    return *this;
}

bool Chunk::IsEmpty() const {
    return blocks.size() == 0;
}

bool Chunk::IsFull() const {
    return blocks.size() == CHUNK_SIZE * CHUNK_SIZE;
}

bool Chunk::BlockExists(const Point3i& blockPos) const {
    return blocks.find(blockPos) != blocks.end();
}

bool Chunk::operator<(const Chunk& rhs) const {
    return GetPosition() < rhs.GetPosition();
}

/* measurements:
    70-100 ms (full 4096 chunk): (OLD) Block visibility checked after generation is done (through map accesses), excluding border blocks
    ~60 ms    (full 4096): (CURRENT) Visibility saved in block, calculated during building (partly using raw pointers), includes border blocks, which need additionial checking
*/
void Chunk::Generate(const Architect& architect) {
    sf::Clock clock;

    HeightArray relativeHeight;
    CalculateHeights(relativeHeight, chunkPos, architect);

    int32_t* currHeight = &relativeHeight[0];
    for (auto y = 0; y < CHUNK_SIZE; y++) {
        for (auto x = 0; x < CHUNK_SIZE; x++) {
            //border column neigbours in other chunks are considered having SIZE height
            //assuming they're full of blocks -> false positives occur (visible blocks classified invisible)
            std::array<int32_t, 4> neighbourHeight = { { CHUNK_SIZE,
                                                       CHUNK_SIZE,
                                                       CHUNK_SIZE,
                                                       CHUNK_SIZE
                                                     } };
            if (x > 0) {
                neighbourHeight[0] = *(currHeight - 1);
            }
            if (y > 0) {
                neighbourHeight[1] = *(currHeight - CHUNK_SIZE);
            }
            if (x + 1 < CHUNK_SIZE) {
                neighbourHeight[2] = *(currHeight + 1);
            }
            if (y + 1 < CHUNK_SIZE) {
                neighbourHeight[3] = *(currHeight + CHUNK_SIZE);
            }
            GenerateColumn(Point3i(x, y, *currHeight), neighbourHeight);
            currHeight++;
        }
    }

    if (!IsEmpty()) {
        CreateMesh();
    }
   
    TRACE("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size());
}


void Chunk::GenerateColumn(Point3i top, const std::array<int32_t, 4>& neighbourHeight) {
    Point3i curr(top);
    static Direction neighbours[] = { Direction::WEST,    // -x
                                      Direction::NORTH,   // -y
                                      Direction::EAST,    // +x
                                      Direction::SOUTH }; // +y

    while (curr[2] >= 0) {
        Block block;

        if (curr[2] == top[2] && top[2] != CHUNK_SIZE - 1) {
            block.AddNeighbour(Direction::DOWN);
        } else {
            block.AddNeighbour(Direction::UP);
            block.AddNeighbour(Direction::DOWN);
        }
        for (auto i = 0; i < 4; i++) {
            if (curr[2] <= neighbourHeight[i]) {
                block.AddNeighbour(neighbours[i]);
            }
        }
        blocks.emplace(curr, std::move(block));
        curr[2] = curr[2] - 1;
    }
}

void Chunk::UpdateBlockVisibility(Direction dir, Chunk& neighbour) {
    assert(!CheckedNeighbour(dir));
    Direction opp = GetOpposite(dir);

    switch (dir) {
        case Direction::NORTH:
            for (auto i = 0; i < CHUNK_SIZE; i++) {
                for (auto j = 0; j < CHUNK_SIZE; j++) {
                    Point3i localBlockPos(i, 0, j);
                    Point3i neighbourBlockPos(i, CHUNK_SIZE - 1, j);

                    auto foundLocal = blocks.find(localBlockPos);
                    auto foundNeighbour = neighbour.blocks.find(neighbourBlockPos);

                    if (foundLocal != blocks.end() && foundNeighbour == blocks.end()) {
                        foundLocal->second.RemoveNeighbour(dir);
                    } else if (foundNeighbour != blocks.end() && foundLocal == blocks.end()) {
                        if (!neighbour.CheckedNeighbour(opp)) {
                            foundNeighbour->second.RemoveNeighbour(opp);
                            neighbour.checkedNeighbours.Add(opp);
                        }
                    }
                }
            }
            break;
        case Direction::UP:
             for (auto i = 0; i < CHUNK_SIZE; i++) {
                for (auto j = 0; j < CHUNK_SIZE; j++) {
                    Point3i localBlockPos(i, j, CHUNK_SIZE - 1);
                    Point3i neighbourBlockPos(i, j, 0);

                    auto foundLocal = blocks.find(localBlockPos);
                    auto foundNeighbour = neighbour.blocks.find(neighbourBlockPos);

                    if (foundLocal != blocks.end() && foundNeighbour == blocks.end()) {
                        foundLocal->second.RemoveNeighbour(dir);
                    } else if (foundNeighbour != blocks.end() && foundLocal == blocks.end()) {
                        if (!neighbour.CheckedNeighbour(opp)) {
                            foundNeighbour->second.RemoveNeighbour(opp);
                            neighbour.checkedNeighbours.Add(opp);
                        }
                    }
                }
            }
            break;
        default: break;
    }
    checkedNeighbours.Add(dir);
}

void Chunk::CreateMesh() {
    mesh = CreateCompositeMesh(blocks);
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

static void CalculateHeights(HeightArray& height,
                      const Point3i chunkPos,
                      const Architect& architect) {


    int32_t chunkMinHeight = chunkPos[2] * CHUNK_SIZE;    //incl
    int32_t chunkMaxHeight = chunkMinHeight + CHUNK_SIZE; //excl
    int32_t* currHeight = &height[0];

    for (auto y = 0; y < CHUNK_SIZE; y++) {
        for (auto x = 0; x < CHUNK_SIZE; x++) {
            *currHeight = architect.GetChunkRelativeHeight(chunkPos, Point2i(x, y));

            if (*currHeight >= chunkMinHeight && *currHeight < chunkMaxHeight) {
                *currHeight -= chunkMinHeight;       // part air/underground col
            } else if (*currHeight >= chunkMaxHeight) {
                *currHeight = CHUNK_SIZE - 1;      // full underground col
            } else {
                *currHeight = -1;                     // full air col
            }
            currHeight++;
        }
    }
}

}       // namespace mc::world::chunk

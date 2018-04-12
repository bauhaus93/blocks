/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

static void CalculateHeights(HeightArray& height,
                    const Point3i chunkPos,
                    const architect::Architect& architect);
//bool IsBorderBlock(const Point3i& blockPos);

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE * BLOCK_SIZE) },
    model {  CreateTranslationMatrix(origin) * glm::mat4(1.0f) },
    blocks { },
    mesh { nullptr } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    origin { other.origin },
    model { std::move(other.model) },
    blocks { std::move(other.blocks) },
    mesh { std::move(other.mesh) } {
}

Chunk& Chunk::operator=(Chunk&& other) {
    chunkPos = other.chunkPos;
    origin = other.origin;
    model = std::move(other.model);
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

bool Chunk::operator<(const Chunk& rhs) const {
    return GetPosition() < rhs.GetPosition();
}

void Chunk::Generate(const architect::Architect& architect) {
    sf::Clock clock;

    HeightArray relativeHeight;
    CalculateHeights(relativeHeight, chunkPos, architect);

    int32_t* currHeight = &relativeHeight[0];
    for (auto y = 0; y < CHUNK_SIZE; y++) {
        for (auto x = 0; x < CHUNK_SIZE; x++) {
            if (*currHeight >= 0) {
                std::array<int32_t, 4> neighbourHeight = { { CHUNK_SIZE,
                                                        CHUNK_SIZE,
                                                        CHUNK_SIZE,
                                                        CHUNK_SIZE
                                                        } };
                if (x > 0) {
                    neighbourHeight[0] = *(currHeight - 1);
                } else {
                    Point3i offset = GetOffset(Direction::WEST);
                    neighbourHeight[0] = architect.GetChunkRelativeHeight(chunkPos + offset, Point2i(CHUNK_SIZE - 1, y));
                }
                if (y > 0) {
                    neighbourHeight[1] = *(currHeight - CHUNK_SIZE);
                } else {
                    Point3i offset = GetOffset(Direction::NORTH);
                    neighbourHeight[1] = architect.GetChunkRelativeHeight(chunkPos + offset, Point2i(x, CHUNK_SIZE - 1)); 
                }
                if (x + 1 < CHUNK_SIZE) {
                    neighbourHeight[2] = *(currHeight + 1);
                } else {
                    Point3i offset = GetOffset(Direction::EAST);
                    neighbourHeight[2] = architect.GetChunkRelativeHeight(chunkPos + offset, Point2i(0, y));
                }
                if (y + 1 < CHUNK_SIZE) {
                    neighbourHeight[3] = *(currHeight + CHUNK_SIZE);
                } else {
                    Point3i offset = GetOffset(Direction::SOUTH);
                    neighbourHeight[3] = architect.GetChunkRelativeHeight(chunkPos + offset, Point2i(x, 0)); 
                }
                GenerateColumn(Point3i(x, y, *currHeight), neighbourHeight, architect);
            }
            currHeight++;
        }
    }
    blocks.shrink_to_fit();

    if (!IsEmpty()) {
        CreateMesh();
    }
   
    TRACE("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size());
}


void Chunk::GenerateColumn(Point3i top,
                           const std::array<int32_t, 4>& neighbourHeight,
                           const architect::Architect& architect) {
    Point3i curr(top);
    static Direction neighbours[] = { Direction::WEST,    // -x
                                      Direction::NORTH,   // -y
                                      Direction::EAST,    // +x
                                      Direction::SOUTH }; // +y
    bool topNeighbour = false;
    if (curr[2] == CHUNK_SIZE - 1) {
        Point3i offset = GetOffset(Direction::UP);
        if (architect.GetChunkRelativeHeight(chunkPos + offset, Point2i(top[0], top[1])) > 0) {
            topNeighbour = true;
        }
    }

    while (curr[2] >= 0) {
        Block block(curr, architect.GetBlockPrototype(chunkPos, curr));

        if (curr[2] != top[2]) {
            block.AddNeighbour(Direction::UP);
        } else {
            if (curr[2] == CHUNK_SIZE - 1 && topNeighbour) {
                block.AddNeighbour(Direction::UP);
            }
        }
        block.AddNeighbour(Direction::DOWN);

        for (auto i = 0; i < 4; i++) {
            if (curr[2] <= neighbourHeight[i]) {
                block.AddNeighbour(neighbours[i]);
            }
        }
        blocks.emplace_back(std::move(block));
        curr[2] = curr[2] - 1;
    }
}

void Chunk::CreateMesh() {
    mesh = CreateCompositeMesh(blocks);
    if (mesh->IsEmpty()) {
        mesh = nullptr;
    }
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

static void CalculateHeights(HeightArray& height,
                      const Point3i chunkPos,
                      const architect::Architect& architect) {

    int32_t* currHeight = &height[0];

    for (auto y = 0; y < CHUNK_SIZE; y++) {
        for (auto x = 0; x < CHUNK_SIZE; x++) {
            *currHeight = architect.GetChunkRelativeHeight(chunkPos, Point2i(x, y));
            currHeight++;
        }
    }
}

}       // namespace mc::world::chunk
